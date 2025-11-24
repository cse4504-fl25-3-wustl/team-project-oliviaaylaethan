#include "packingInteractor.h"
#include <limits>
#include <algorithm> // std::sort

using namespace std;


PackingInteractor::PackingInteractor() {
    boxes_ = std::vector<Box>();
    pallets_ = std::vector<ShippingContainer>();
    crates_ = std::vector<ShippingContainer>();
}

float PackingInteractor::computeMinTareWeight(int numBoxes) {
    // dp[i] = minimum tare weight to hold i boxes
    std::vector<float> dp(numBoxes + 1, std::numeric_limits<float>::infinity());
    dp[0] = 0.0f;

    for (int i = 1; i <= numBoxes; ++i) {
        if (i >= STANDARD_PALLET_STANDARD_BOX_CAPACITY)
            dp[i] = std::min(dp[i], dp[i - STANDARD_PALLET_STANDARD_BOX_CAPACITY] + STANDARD_PALLET_TARE_WEIGHT);
        if (i >= OVERSIZE_PALLET_STANDARD_BOX_CAPACITY)
            dp[i] = std::min(dp[i], dp[i - OVERSIZE_PALLET_STANDARD_BOX_CAPACITY] + OVERSIZE_PALLET_TARE_WEIGHT);

        // if less than 4 boxes left, we still need one pallet
        if (i < STANDARD_PALLET_STANDARD_BOX_CAPACITY)
            dp[i] = std::min(dp[i], STANDARD_PALLET_TARE_WEIGHT);
    }

    return dp[numBoxes];
}

Response PackingInteractor::packAllArt(Request request) {
    bool allowCrates = request.getRequirements().getAcceptsCrates().value_or(true);

    // If crates are allowed, we do not pack into boxes
    if (!allowCrates) {
        vector<Art> needsStandardBox = vector<Art>();
        vector<Art> needsLargeBox = vector<Art>();
        vector<Art> needsCrate = vector<Art>();
        vector<Art> needsLargeCrate = vector<Art>();
        vector<Art> needsCustomPallet = vector<Art>();

        Box standardBox = Box::makeStandardBox();
        Box largeBox = Box::makeLargeBox();

        // segment art pieces by size and material
        for (Art piece : request.getArtPieces()) {
            if ((piece.getMaterial() == MIRROR || (piece.needsCratePacking() && !piece.needsLargeCrateCapacity())) && allowCrates) {
                needsStandardBox.push_back(piece);
            } else if (standardBox.fitsArt(piece)) {
                needsStandardBox.push_back(piece);
            } else if (largeBox.fitsArt(piece)) {
                needsLargeBox.push_back(piece);
            } else {
                needsCustomPallet.push_back(piece);
            }
        }
        
        // Pack large boxes FIRST
        size_t i = 0;
        if (!needsLargeBox.empty()) {
            while (i < needsLargeBox.size()) {
                bool added = largeBox.addArt(needsLargeBox[i]);
                if (added) {
                    // successfully added, move to next piece
                    ++i;
                } else {
                    // box full (by fraction or capacity), push it and start a new one
                    boxes_.push_back(largeBox);
                    largeBox = Box::makeLargeBox();
                }
            }

            // add some pieces that COULD fit in a standard box to a large box with extra capacity (to save space)
            while (!needsStandardBox.empty() && largeBox.getFilledFrac() < 1.0f) {
                if (!largeBox.addArt(needsStandardBox.back())) break;
                needsStandardBox.pop_back();
            }

            
            // don’t forget to push the last partially-filled box
            if (!largeBox.getContents().empty()) {
                boxes_.push_back(largeBox);
            }
        }

        // Pack standard boxes
        i = 0;
        while (i < needsStandardBox.size()) {
            bool added = standardBox.addArt(needsStandardBox[i]);
            if (added) {
                // successfully added, move to next piece
                ++i;
            } else {
                // box full (by fraction or capacity), push it and start a new one
                boxes_.push_back(standardBox);
                standardBox = Box::makeStandardBox();
            }
        }

        if (!standardBox.getContents().empty()) {
            boxes_.push_back(standardBox);
        }

        packIntoPallets();

    } else {
        packIntoCrates(request.getArtPieces());
    }


    // Material	If <33" both dimensions	If >33" either dimension
    // Glass/Acrylic	25 pieces per crate	18 pieces per crate
    // Canvas	18 pieces per crate	12 pieces per crate
    // Mirrors	24-25 pieces directly in crate	(no boxes)
    // const int CRATE_LARGE_THRESHOLD = 33;
    // const int GLASS_ACRYLIC_SMALL_CRATE_CAPACITY = 25;
    // const int GLASS_ACRYLIC_LARGE_CRATE_CAPACITY = 18;
    // const int CANVAS_SMALL_CRATE_CAPACITY = 18;
    // const int CANVAS_LARGE_CRATE_CAPACITY = 12;
    // const int MIRROR_CRATE_CAPACITY = 24;


    // Commenting out mirror code for now
    
    // // Pack mirrors in crates
    // for (size_t i = 0; i < needsCrate.size(); i += GLASS_ACRYLIC_SMALL_CRATE_CAPACITY) {
    //     ShippingContainer crate = ShippingContainer::makeStandardCrate();
    //     for (size_t j = i; j < i + GLASS_ACRYLIC_SMALL_CRATE_CAPACITY && j < needsCrate.size(); ++j) {
    //         crate.addArt(needsCrate[j]);
    //     }
    //     crates_.push_back(crate);
    // }

    // for (size_t i = 0; i < needsLargeCrate.size(); i += GLASS_ACRYLIC_SMALL_CRATE_CAPACITY) {
    //     ShippingContainer crate = ShippingContainer::makeStandardCrate();
    //     for (size_t j = i; j < i + GLASS_ACRYLIC_SMALL_CRATE_CAPACITY && j < needsLargeCrate.size(); ++j) {
    //         crate.addArt(needsLargeCrate[j]);
    //     }
    //     crates_.push_back(crate);
    // }


    return Response(boxes_, pallets_, crates_, request.getRequirements(), request.getArtPieces());
}

void PackingInteractor::packIntoPallets() {
    vector<Box> ordered = vector<Box>();

    for (int i = 0; i < boxes_.size(); i++) {
        if (boxes_[i].getBoxType() == BoxType::LARGE_BOX) {
            ordered.insert(ordered.begin(), boxes_[i]);
        } else {
            ordered.push_back(boxes_[i]);
        }
    }

    vector<Box> tempBoxes = ordered;
    int remaining = tempBoxes.size();

    while (remaining > 0) {
        // Check which choice yields lower total tare weight
        float useStandard = (remaining >= STANDARD_PALLET_STANDARD_BOX_CAPACITY)
            ? computeMinTareWeight(remaining - STANDARD_PALLET_STANDARD_BOX_CAPACITY) + STANDARD_PALLET_TARE_WEIGHT
            : std::numeric_limits<float>::infinity();

        float useOversized = (remaining >= OVERSIZE_PALLET_STANDARD_BOX_CAPACITY)
            ? computeMinTareWeight(remaining - OVERSIZE_PALLET_STANDARD_BOX_CAPACITY) + OVERSIZE_PALLET_TARE_WEIGHT
            : std::numeric_limits<float>::infinity();

        ShippingContainer pallet;
        int capacity = 0;

        if (useStandard < useOversized || remaining < OVERSIZE_PALLET_STANDARD_BOX_CAPACITY) {
            pallet = ShippingContainer::makeStandardPallet();
        } else {
            pallet = ShippingContainer::makeOversizePallet();
        }

        capacity = pallet.getStandardBoxCapacity();

        for (int j = 0; j < capacity && !tempBoxes.empty(); ++j) {
            if (tempBoxes.back().getBoxType() == BoxType::LARGE_BOX) {
                capacity = pallet.getOversizedBoxCapacity();
            }
            pallet.addBox(tempBoxes.back());
            tempBoxes.pop_back();
        }

        pallets_.push_back(std::move(pallet));
        remaining = tempBoxes.size();
    }
}

void PackingInteractor::packIntoCrates(vector<Art> artPieces) {
    if (artPieces.empty()) return;

    std::sort(artPieces.begin(), artPieces.end(),
          [](Art& a, Art& b) {
              if (a.getPerCrateCount() != b.getPerCrateCount())
                  return a.getPerCrateCount() > b.getPerCrateCount();
              return (a.getOuterWidth() * a.getOuterHeight()) >
                     (b.getOuterWidth() * b.getOuterHeight());
          });

    ShippingContainer crate = ShippingContainer::makeStandardCrate();

    while (!artPieces.empty()) {
        if (crate.addArt(artPieces.back())) {
            artPieces.pop_back();
        } else {
            crates_.push_back(std::move(crate));
            crate = ShippingContainer::makeStandardCrate();
        }
    }

    // Only push the final crate if it contains something
    if (!crate.getArtContents().empty()) {
        crates_.push_back(std::move(crate));
    }
}

std::vector<Box> PackingInteractor::getBoxes() {
    return boxes_;
}

std::vector<ShippingContainer> PackingInteractor::getPallets() {
    return pallets_;
}

std::vector<ShippingContainer> PackingInteractor::getCrates() {
    return crates_;
}

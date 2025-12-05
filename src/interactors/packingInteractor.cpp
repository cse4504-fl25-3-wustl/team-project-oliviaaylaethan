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
            while (!needsStandardBox.empty() && largeBox.canFitMore()) {
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


    return Response(boxes_, pallets_, crates_, std::make_shared<Requirements>(request.getRequirements()), request.getArtPieces());
}

void PackingInteractor::packIntoPallets() {
    vector<Box> standardBoxes = vector<Box>();
    vector<Box> largeBoxes = vector<Box>();

    for (int i = 0; i < boxes_.size(); i++) {
        if (boxes_[i].getBoxType() == BoxType::LARGE_BOX) {
            largeBoxes.push_back(boxes_[i]);
        } else {
            standardBoxes.push_back(boxes_[i]);
        }
    }

    // all the large boxes should go on standard pallets because only 3 large boxes fit on a pallet,
    // regardless of whether the pallet is standard or oversize (and oversize pallets are heavier)
    while (largeBoxes.size() > 0) {
        ShippingContainer pallet = ShippingContainer::makeStandardPallet();
        int capacity = STANDARD_PALLET_OVERSIZED_BOX_CAPACITY;
        for (int j = 0; j < capacity && !largeBoxes.empty(); ++j) {
            pallet.addBox(largeBoxes.back());
            largeBoxes.pop_back();
        }
        // if there is still space on the pallet, fill it with standard boxes
        // (this will only happen if the number of large boxes isn't divisible by 3)
        if (pallet.getContents().size() < capacity) {
            while (!standardBoxes.empty() && pallet.getContents().size() < capacity) {
                pallet.addBox(standardBoxes.back());
                standardBoxes.pop_back();
            }
        }
        pallets_.push_back(std::move(pallet));
    }

    // standard boxes could go on either standard or oversize pallets
    vector<Box> tempBoxes = standardBoxes;
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
        
        if (useStandard < useOversized || remaining < OVERSIZE_PALLET_STANDARD_BOX_CAPACITY) {
            pallet = ShippingContainer::makeStandardPallet();
        } else {
            pallet = ShippingContainer::makeOversizePallet();
        }

        int capacity = pallet.getStandardBoxCapacity(); // everything from here on out is standard boxes

        for (int j = 0; j < capacity && !tempBoxes.empty(); ++j) {
            pallet.addBox(tempBoxes.back());
            tempBoxes.pop_back();
        }

        pallets_.push_back(std::move(pallet));
        remaining = tempBoxes.size();
    }
}

void PackingInteractor::packIntoCrates(vector<Art> artPieces) {
    if (artPieces.empty()) return;

    // Determine material-specific capacities
    auto getCaps = [](MaterialType m) {
        struct Caps { int longCap; int shortCap; } caps;
        if (m == MaterialType::CANVAS_FRAMED || m == MaterialType::CANVAS_GALLERY || m == MaterialType::ACOUSTIC_PANEL || m == MaterialType::ACOUSTIC_PANEL_FRAMED) {
            caps.longCap = CANVAS_SMALL_CRATE_CAPACITY; // 18
            caps.shortCap = CANVAS_LARGE_CRATE_CAPACITY; // 14
        } else if (m == MaterialType::PAPER_PRINT_FRAMED) { // glass/acrylic framed
            caps.longCap = GLASS_ACRYLIC_SMALL_CRATE_CAPACITY; // 25
            caps.shortCap = GLASS_ACRYLIC_LARGE_CRATE_CAPACITY; // 19
        } else {
            // default to canvas-like behavior if unspecified
            caps.longCap = CANVAS_SMALL_CRATE_CAPACITY;
            caps.shortCap = CANVAS_LARGE_CRATE_CAPACITY;
        }
        return caps;
    };

    // Split by capability: long-only, short-only, and flexible (fits both)
    std::vector<Art> longOnly;
    std::vector<Art> shortOnly;
    std::vector<Art> flex;

    longOnly.reserve(artPieces.size());
    shortOnly.reserve(artPieces.size());
    flex.reserve(artPieces.size());

    for (auto &a : artPieces) {
        float w = a.getOuterWidth();
        float h = a.getOuterHeight();
        float shorter = std::min(w, h);
        float longer  = std::max(w, h);
        bool canLong = (shorter < LARGE_CRATE_CAPACITY_WIDTH_THRESHOLD);
        bool canShort = (shorter <= LARGE_CRATE_CAPACITY_HEIGHT_THRESHOLD && longer <= LARGE_CRATE_CAPACITY_WIDTH_THRESHOLD) || (shorter >= LARGE_CRATE_CAPACITY_WIDTH_THRESHOLD);

        if (canLong && canShort && shorter <= LARGE_CRATE_CAPACITY_HEIGHT_THRESHOLD && longer <= LARGE_CRATE_CAPACITY_WIDTH_THRESHOLD) {
            flex.push_back(a); // clearly fits both within footprint window
        } else if (canLong && !canShort) {
            longOnly.push_back(a);
        } else if (!canLong && canShort) {
            shortOnly.push_back(a);
        } else {
            // default to long if ambiguous for any reason
            longOnly.push_back(a);
        }
    }

    // Determine capacities based on the (assumed uniform) material of the batch
    MaterialType mat = artPieces.front().getMaterial();
    auto caps = getCaps(mat);

    auto fill_crates = [&](std::vector<Art> &primary, std::vector<Art> &flexible, int cap) {
        while (!primary.empty()) {
            ShippingContainer crate = ShippingContainer::makeStandardCrate();
            int added = 0;
            // Fill from primary first
            size_t i = 0;
            while (i < primary.size()) {
                if (!crate.addArtWithCapacity(primary[i], cap)) break;
                ++added;
                primary[i] = std::move(primary.back());
                primary.pop_back();
            }
            // Top-up from flexible if space remains
            i = 0;
            while (i < flexible.size() && crate.getFilledFrac() + (1.0f / cap) - 1.0f <= EPS) {
                if (!crate.addArtWithCapacity(flexible[i], cap)) break;
                ++added;
                flexible[i] = std::move(flexible.back());
                flexible.pop_back();
            }
            if (added > 0) crates_.push_back(std::move(crate));
            else break;
        }
    };

    // Prefer filling long-capacity crates first to minimize total crates, then short
    fill_crates(longOnly, flex, caps.longCap);
    fill_crates(shortOnly, flex, caps.shortCap);

    // Any remaining flexible pieces: pack them using long capacity to minimize crate count
    while (!flex.empty()) {
        ShippingContainer crate = ShippingContainer::makeStandardCrate();
        int cap = caps.longCap;
        int added = 0;
        size_t i = 0;
        while (i < flex.size()) {
            if (!crate.addArtWithCapacity(flex[i], cap)) break;
            ++added;
            flex[i] = std::move(flex.back());
            flex.pop_back();
        }
        if (added > 0) crates_.push_back(std::move(crate));
        else break;
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

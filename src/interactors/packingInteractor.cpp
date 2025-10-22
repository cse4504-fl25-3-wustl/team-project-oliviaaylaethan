#include "packingInteractor.h"

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
    vector<Art> needsStandardBox = vector<Art>();
    vector<Art> needsLargeBox = vector<Art>();
    vector<Art> needsMirrorPacking = vector<Art>();
    vector<Art> needsCustomPallet = vector<Art>();

    Box standardBox = Box::makeStandardBox();
    Box largeBox = Box::makeLargeBox();

    // segment art pieces by size and material
    for (Art piece : request.getArtPieces()) {
        if (piece.getMaterial() == MIRROR) {
            needsMirrorPacking.push_back(piece); // Mirrors always use crates
        } else if (standardBox.fitsArt(piece)) {
            needsStandardBox.push_back(piece);
        } else if (largeBox.fitsArt(piece)) {
            needsLargeBox.push_back(piece);
        } else {
            needsCustomPallet.push_back(piece);
        }
    }

    // Pack standard boxes
    for (size_t i = 0; i < needsStandardBox.size(); i += STANDARD_BOX_CAPACITY) {
        Box box = Box::makeStandardBox();
        for (size_t j = i; j < i + STANDARD_BOX_CAPACITY && j < needsStandardBox.size(); ++j) {
            box.addArt(needsStandardBox[j]);
        }
        boxes_.push_back(box);
    }

    // Pack large boxes
    for (size_t i = 0; i < needsLargeBox.size(); i += LARGE_BOX_CAPACITY) {
        Box box = Box::makeLargeBox();
        for (size_t j = i; j < i + LARGE_BOX_CAPACITY && j < needsLargeBox.size(); ++j) {
            box.addArt(needsLargeBox[j]);
        }
        boxes_.push_back(box);
    }

    // Place all boxes on pallets
    vector<Box> tempBoxes = boxes_;
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

        if (useStandard <= useOversized || remaining < OVERSIZE_PALLET_STANDARD_BOX_CAPACITY) {
            pallet = ShippingContainer::makeStandardPallet();
            capacity = STANDARD_PALLET_STANDARD_BOX_CAPACITY;
        } else {
            pallet = ShippingContainer::makeOversizePallet();
            capacity = OVERSIZE_PALLET_STANDARD_BOX_CAPACITY;
        }

        for (int j = 0; j < capacity && !tempBoxes.empty(); ++j) {
            pallet.addBox(tempBoxes.back());
            tempBoxes.pop_back();
        }

        pallets_.push_back(std::move(pallet));
        remaining = tempBoxes.size();
    }   


    // Pack custom crates and pallets based on material rules
    for (Art piece : needsCustomPallet) {
        if (piece.getMaterial() == MIRROR) {
            ShippingContainer crate = ShippingContainer::makeStandardCrate();
            Box mirrorBox = Box::makeLargeBox();
            mirrorBox.addArt(piece);
            crate.addBox(mirrorBox); // Use addBox instead of directly modifying contents_
            crates_.push_back(crate);
        } else {
            Box customBox = Box::makeLargeBox();
            customBox.addArt(piece);
            boxes_.push_back(customBox);

            ShippingContainer pallet = ShippingContainer::makeOversizePallet();
            pallet.addBox(customBox);
            pallets_.push_back(pallet);
        }
    }

    // Handle Canvas Rule Discrepancy
    // 🚨 FLAG: Canvas packing rule discrepancy. Using Excel logic (12 per pallet) for now.
    // for (size_t i = 0; i < needsStandardBox.size(); ++i) {
    //     if (needsStandardBox[i].needsCanvasPacking()) {
    //         ShippingContainer pallet = ShippingContainer::makeStandardPallet();
    //         for (size_t j = i; j < i + STANDARD_PALLET_CANVAS_BOX_CAPACITY && j < needsStandardBox.size(); ++j) {
    //             if (needsStandardBox[j].needsCanvasPacking()) {
    //                 //pallet.addBox(boxes_[j]); // FIXME crashing program bc going out of bounds (at least for input1)
    //             }
    //         }
    //         pallets_.push_back(pallet);
    //     }
    // }

    return Response(boxes_, pallets_, crates_, request.getRequirements());
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

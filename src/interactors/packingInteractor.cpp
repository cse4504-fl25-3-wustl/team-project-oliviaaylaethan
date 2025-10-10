#include "packingInteractor.h"

using namespace std;


PackingInteractor::PackingInteractor() {
    boxes_ = std::vector<Box>();
    pallets_ = std::vector<ShippingContainer>();
    crates_ = std::vector<ShippingContainer>();
}

Response PackingInteractor::packAllArt(Request request) {
    vector<Art> needsStandardBox = vector<Art>();
    vector<Art> needsLargeBox = vector<Art>();
    vector<Art> needsCustomPallet = vector<Art>();

    // segment art pieces by size and material
    for (Art piece : request.getArtPieces()) {
        if (piece.getMaterial() == MaterialType::MIRROR) {
            needsCustomPallet.push_back(piece); // Mirrors always use crates
        } else if (piece.getOuterWidth() <= 36 && piece.getOuterHeight() <= 36) {
            needsStandardBox.push_back(piece);
        } else if (piece.getOuterWidth() <= 43.5 && piece.getOuterHeight() <= 43.5) {
            needsLargeBox.push_back(piece);
        } else {
            needsCustomPallet.push_back(piece);
        }
    }

    // Pack standard boxes
    for (size_t i = 0; i < needsStandardBox.size(); i += 6) { // 6 pieces per standard box
        Box box = Box::makeStandardBox();
        for (size_t j = i; j < i + 6 && j < needsStandardBox.size(); ++j) {
            box.addArt(needsStandardBox[j]);
        }
        boxes_.push_back(box);
    }

    // Pack large boxes
    for (size_t i = 0; i < needsLargeBox.size(); i += 4) { // 4 pieces per large box
        Box box = Box::makeLargeBox();
        for (size_t j = i; j < i + 4 && j < needsLargeBox.size(); ++j) {
            box.addArt(needsLargeBox[j]);
        }
        boxes_.push_back(box);
    }

    // Place all boxes on pallets
    for (size_t i = 0; i < boxes_.size(); i += 4) { // 4 boxes per pallet
        ShippingContainer pallet = ShippingContainer::makeStandardPallet();
        for (size_t j = i; j < i + 4 && j < boxes_.size(); ++j) {
            pallet.addBox(boxes_[j]);
        }
        pallets_.push_back(pallet);
    }

    // Pack custom crates and pallets based on material rules
    for (Art piece : needsCustomPallet) {
        if (piece.getMaterial() == MaterialType::MIRROR) {
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
    for (size_t i = 0; i < needsStandardBox.size(); ++i) {
        if (needsStandardBox[i].getMaterial() == MaterialType::CANVAS_FRAMED ||
            needsStandardBox[i].getMaterial() == MaterialType::CANVAS_GALLERY) {
            ShippingContainer pallet = ShippingContainer::makeStandardPallet();
            for (size_t j = i; j < i + 12 && j < needsStandardBox.size(); ++j) {
                if (needsStandardBox[j].getMaterial() == MaterialType::CANVAS_FRAMED ||
                    needsStandardBox[j].getMaterial() == MaterialType::CANVAS_GALLERY) {
                    pallet.addBox(boxes_[j]);
                }
            }
            pallets_.push_back(pallet);
        }
    }

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

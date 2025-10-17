#include "box.h"

Box::Box() : dimensions_{0, 0, 0}, boxType_(STANDARD_BOX), totalWeight_(0) {
    contents_ = std::vector<Art>();
}

Box::Box(Dimensions dimensions, BoxType boxType) {
    dimensions_ = dimensions;
    boxType_ = boxType;
}

Box Box::makeStandardBox() {
    return Box(STANDARD_BOX_DIMENSIONS, STANDARD_BOX);
}

Box Box::makeLargeBox() {
    return Box(LARGE_BOX_DIMENSIONS, LARGE_BOX);
}

Box Box::makeUPSSmallBox() {
    return Box(UPS_SMALL_BOX_DIMENSIONS, UPS_SMALL_BOX);
}

Box Box::makeUPSLargeBox() {
    return Box(UPS_LARGE_BOX_DIMENSIONS, UPS_LARGE_BOX);
}

bool Box::fitsArt(Art artwork) {
    // Rule: As long as at least ONE dimension of an art piece is 36" or less, it will fit in a standard size box. Boxes can be telescoped to a max height of 84"
    if (boxType_ == STANDARD_BOX) {
        if (artwork.getOuterHeight() <= STANDARD_BOX_MAX_ART_DIMENSION || artwork.getOuterWidth() <= STANDARD_BOX_MAX_ART_DIMENSION) {
            return true;
        }
        return false;
    }
    else { // TODO check real rules for other box types
        if (artwork.getOuterHeight() < dimensions_.l && artwork.getOuterWidth() < dimensions_.h) {
            return true;
        } 
        if (artwork.getOuterWidth() < dimensions_.l && artwork.getOuterHeight() < dimensions_.h) {
            return true;
        }
        return false;
    }
}

bool Box::addArt(Art art) {
    if(!fitsArt(art) || (boxType_ == STANDARD_BOX && contents_.size() >= STANDARD_BOX_CAPACITY) || (boxType_ == LARGE_BOX && contents_.size() >= LARGE_BOX_CAPACITY)) {
        return false;
    }
    contents_.push_back(art);
    totalWeight_ += art.getWeight();
    return true;
};

Dimensions Box::getDimensions() const {
    return dimensions_;
}

BoxType Box::getBoxType() const {
    return boxType_;
}

int Box::getTotalWeight() const { 
    return totalWeight_; 
}

std::vector<Art> Box::getContents() const {
    return contents_;
}
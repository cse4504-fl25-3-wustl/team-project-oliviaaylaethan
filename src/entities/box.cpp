#include "box.h"

Box::Box() : dimensions_{0, 0, 0}, boxType_(STANDARD_BOX), totalWeight_(0) {
    contents_ = std::vector<Art>();
    capacity_ = STANDARD_BOX_CAPACITY;
}

Box::Box(Dimensions dimensions, BoxType boxType) : dimensions_(dimensions), boxType_(boxType), totalWeight_(0) {
    contents_ = std::vector<Art>();
    if (boxType_ == STANDARD_BOX) {
        capacity_ = STANDARD_BOX_CAPACITY;
    }
    else if (boxType_ == LARGE_BOX) {
        capacity_ = LARGE_BOX_CAPACITY;
    }
    else {
        capacity_ = 6;
    }
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
    // Rule: As long as at least ONE dimension of an art piece is 36" or less, it will fit in a standard size box. Boxes can be telescoped to a max height of 88"
    if (boxType_ == STANDARD_BOX) {
        bool shortEnough = (artwork.getOuterHeight() <= MAX_BOX_HEIGHT && artwork.getOuterWidth() <= MAX_BOX_HEIGHT);
        bool couldFitWithTelescoping = (artwork.getOuterHeight() <= STANDARD_BOX_TELESCOPED_THRESHOLD || artwork.getOuterWidth() <= STANDARD_BOX_TELESCOPED_THRESHOLD);
        if (shortEnough && couldFitWithTelescoping) {
            return true;
        }
        return false;
    }
    else if (boxType_ == LARGE_BOX) {
        // if art does NOT need custom packaging, it should be able to fit in a large box
        return !artwork.needsCustomPackaging(LARGE_BOX_LIMIT);
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
    float fraction = 1.0f / art.getPerBoxCount();
    if(!fitsArt(art) || (contents_.size() >= capacity_) || (filledArt_ + fraction > 1.0f)) {
        return false;
    }
    contents_.push_back(art);
    totalWeight_ += art.getWeight();
    filledArt_ += fraction;

    // update height of box to reflect tallest artwork inside of it TODO ask about rules for nonstandard boxes
    bool needsTelescoping = false;
    if (boxType_ == STANDARD_BOX) {
        needsTelescoping = (art.getOuterHeight() > STANDARD_BOX_TELESCOPED_THRESHOLD || art.getOuterWidth() > STANDARD_BOX_TELESCOPED_THRESHOLD);
    }
    else if (boxType_ == LARGE_BOX) {
        needsTelescoping = TODO_PLACEHOLDER_BOOL; // TODO FIXME ask if large boxes can be telescoped
    }
    if (needsTelescoping) {
        float artTelescopedHeight;
        if ( art.getOuterHeight() > art.getOuterWidth() ) {
            artTelescopedHeight = art.getOuterHeight();
        }
        else {
            artTelescopedHeight = art.getOuterWidth();
        }

        if ( artTelescopedHeight > dimensions_.h ) {
            dimensions_.h = artTelescopedHeight;
        }
    }
    
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
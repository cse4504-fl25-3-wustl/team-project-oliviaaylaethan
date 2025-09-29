#include "crate.h"

Crate::Crate() : dimensions_{0, 0, 0}, totalWeight_(0) {
    contents_ = std::vector<Box>();
}

Crate::Crate(Dimensions dimensions, float tareWeight) {
    dimensions_ = dimensions;
    //tareWeight_ = tareWeight;
}

Crate Crate::makeStandardCrate() {
    return Crate(STANDARD_CRATE_DIMENSIONS, STANDARD_CRATE_TARE_WEIGHT);
}

float Crate::calculateCrateHeight(float largestDimension) {
    return largestDimension + 8; // Add 8 inches buffer
}

#pragma once

#include "box.h"


const Dimensions STANDARD_CRATE_DIMENSIONS = {50, 38, 0};
const float STANDARD_CRATE_TARE_WEIGHT = 125;

// Height calculation for crates
inline float calculateCrateHeight(float largestDimension) {
    return largestDimension + 8; // Add 8 inches buffer
}

class Crate {
    private:
        Dimensions dimensions_;
        float tareWeight_; // in lbs
    
    public:
        Crate(Dimensions dimensions, float tareWeight);
        Crate makeStandardCrate();
        float calculateCrateHeight(float largestDimension);
};

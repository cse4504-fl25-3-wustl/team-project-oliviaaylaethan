#pragma once
#include "box.h"
#include <vector>

// For standard shipments
const Dimensions STANDARD_PALLET_DIMENSIONS = {48, 40, 0};
const float STANDARD_PALLET_WEIGHT = 60;
const int STANDARD_PALLET_STANDARD_BOX_CAPACITY = 4;

// For small shipments
const Dimensions GLASS_SMALL_PALLET_DIMENSIONS = {43, 35, 0};
const float GLASS_SMALL_PALLET_WEIGHT = 60;
const int GLASS_SMALL_PALLET_STANDARD_BOX_CAPACITY = 0;

// Oversize pallet: 5 standard boxes
const Dimensions OVERSIZE_PALLET_DIMENSIONS = {60, 40, 0};
const float OVERSIZE_PALLET_WEIGHT = 75;
const int OVERSIZE_PALLET_STANDARD_BOX_CAPACITY = 5;

const int TEMP_OVERSIZED_BOX_CAPACITY = 3;

class Pallet {
private:
    Dimensions dimensions_;
    int standardBoxCapacity_;
    int oversizedBoxCapacity_; // TODO
    int totalWeight_;
    std::vector<Box> contents_;


public:
    Pallet();
    Pallet(Dimensions dimensions, float weight, int standardBoxCapacity, int oversizedBoxCapacity);
    Pallet makeStandardPallet();
    Pallet makeGlassSmallPallet(); // TODO: What is glass small pallet?
    Pallet makeOversizePallet();
    std::vector<Box> getContents();
};

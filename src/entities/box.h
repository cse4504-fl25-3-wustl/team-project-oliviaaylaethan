#pragma once
#include "art.h"
#include <vector>

struct Dimensions {
    float l, w, h; // Length, Width, Height in inches
};

enum BoxType {
    STANDARD_BOX,
    LARGE_BOX,
    UPS_SMALL_BOX,
    UPS_LARGE_BOX,
    CUSTOM
};

const Dimensions STANDARD_BOX_DIMENSIONS = {37, 11, 31};
const Dimensions LARGE_BOX_DIMENSIONS = {44, 13, 48};
const Dimensions UPS_SMALL_BOX_DIMENSIONS = {36, 6, 36};
const Dimensions UPS_LARGE_BOX_DIMENSIONS = {44, 6, 35};

//static BoxType boxFit(Art art);

class Box {
private:
    Dimensions dimensions_;
    BoxType boxType_;
    std::vector<Art> contents_;
    int totalWeight_;

public:
    Box();
    Box(Dimensions dimensions, BoxType boxType);
    bool addArt(Art art);
    Box makeStandardBox();
    Box makeLargeBox();
    Box makeUPSSmallBox();
    Box makeUPSLargeBox();
    int getTotalWeight();

};

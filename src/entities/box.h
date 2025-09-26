#pragma once

struct Dimensions {
    float l, w, h; // Length, Width, Height in inches
};

enum BoxType {
    STANDARD_BOX,
    LARGE_BOX,
    UPS_SMALL_BOX,
    UPS_LARGE_BOX
};

const Dimensions STANDARD_BOX_DIMENSIONS = {37, 11, 31};
const Dimensions LARGE_BOX_DIMENSIONS = {44, 13, 48};
const Dimensions UPS_SMALL_BOX_DIMENSIONS = {36, 6, 36};
const Dimensions UPS_LARGE_BOX_DIMENSIONS = {44, 6, 35};

class Box {
    private:
        Dimensions dimensions_;
        BoxType boxType_;

    public:
        Box(Dimensions dimensions, BoxType boxType);
        Box makeStandardBox();
        Box makeLargeBox();
        Box makeUPSSmallBox();
        Box makeUPSLargeBox();

};

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
    int totalWeight_;
    std::vector<Art> contents_;

public:
    Box();
    Box(Dimensions dimensions, BoxType boxType);

    static Box makeStandardBox();
    static Box makeLargeBox();
    static Box makeUPSSmallBox();
    static Box makeUPSLargeBox();

    // TODO move all these definitions to box.cpp
    bool addArt(Art art) {
        contents_.push_back(art);
        return true;
    };

    Dimensions getDimensions() const {
        return dimensions_;
    }

    BoxType getBoxType() const {
        return boxType_;
    }

    int getTotalWeight() const { return 100; }
    
    std::vector<Art> getContents() const {
        return contents_;
    }
};

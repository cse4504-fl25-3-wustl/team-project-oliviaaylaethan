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
    CRATE_BOX, // can fit art 36 x 46, 1 single box goes in crate, only use if client accepts crate
    CUSTOM
};

const Dimensions STANDARD_BOX_DIMENSIONS = {37, 11, 31};
const Dimensions LARGE_BOX_DIMENSIONS = {44, 13, 48};
const Dimensions UPS_SMALL_BOX_DIMENSIONS = {36, 6, 36};
const Dimensions UPS_LARGE_BOX_DIMENSIONS = {44, 6, 35};

const int STANDARD_BOX_CAPACITY = 6; // TODO rename this STANDARD_BOX_ART_CAPACITY so it doesn't get mixed up w # of standard boxes that can go on pallet
const int LARGE_BOX_CAPACITY = 6; 

// Rule: As long as at least ONE dimension of an art piece is 36" or less, it will fit in a standard size box. Boxes can be telescoped to a max height of 88"
const int MAX_BOX_HEIGHT = CUSTOM_PACKING_NEEDED_THRESHOLD_LARGER_DIM;
const float STANDARD_BOX_TELESCOPED_THRESHOLD = 36.5f; // Anything that exceeds 36" in ONLY one direction can be telescoped in a standard box.

const bool TODO_PLACEHOLDER_BOOL = false; // for things we don't know the value of yet and need to ask client about for clarification

class Box {
private:
    Dimensions dimensions_;
    BoxType boxType_;
    int totalWeight_;
    std::vector<Art> contents_;
    float filledFrac_ = 0.0f;
    int capacity_;

public:
    Box();
    Box(Dimensions dimensions, BoxType boxType);

    static Box makeStandardBox();
    static Box makeLargeBox();
    static Box makeUPSSmallBox();
    static Box makeUPSLargeBox();

    Dimensions getDimensions() const;
    BoxType getBoxType() const;
    int getTotalWeight() const;
    std::vector<Art> getContents() const;
    float getFilledFrac() const;

    bool fitsArt(Art artwork);
    bool addArt(Art artwork);
};

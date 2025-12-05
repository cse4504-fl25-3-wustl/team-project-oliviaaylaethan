#pragma once
#include "art.h"
#include "../constants/boxConstants.h"
#include <vector>

class Box {
private:
    Dimensions dimensions_;
    BoxType boxType_;
    int totalWeight_;
    std::vector<Art> contents_;
    float filledDepth_;
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

    bool fitsArt(Art artwork);
    bool addArt(Art artwork);
    bool canFitMore(float extraDepth = 0.0f);
};

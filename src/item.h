#pragma once

#include "references/materialsInfo.h"
#include <string>

class Item {
// private variables end with _ to differentiate them (you can only directly use them in item.cpp, otherwise use the public functions like getLineNumber())
private:
    int lineNumber_;
    int quantity_;
    std::string tagNumber_;
    MaterialType material_;
    float materialDensity_;
    float outerWidth_;
    float outerHeight_;
    GlazingType glazeType_;
    std::string frame1Moulding_; // TODO if this impacts calculations, change from string
    std::string hardware_;       // TODO if this impacts calculations, change from string

public:
    // constructor ("density" isn't in here because you'll set that value based on "material")
    Item(int lineNo, int quantity, std::string tagNo, 
        MaterialType material, float outerWidth, float outerHeight, 
        GlazingType glazeType, std::string frame1Moulding, std::string hardware);

	// gets values of private instance variables so other files can use them
	int getLineNumber();
    int getQuantity();
    std::string getTagNumber();
    MaterialType getMaterial();
    float getDensity();
    float getOuterWidth();
    float getOuterHeight();
    GlazingType getGlazeType();
    std::string getFrame1Moulding();
    std::string getHardware();
};

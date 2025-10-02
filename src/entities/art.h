#pragma once

#include <string>

// MATERIAL DENSITY REFERENCE TABLE
// Critical Data - Use these precise densities for ALL calculations

//          Material	 |   Density (LB/SQIN)
const float ACOUSTIC_PANEL_DENSITY = 0.0038;
const float ACOUSTIC_PANEL_FRAMED_DENSITY =	0.0037;
const float ACRYLIC_DENSITY = 0.0094;
const float CANVAS_FRAMED_DENSITY  = 0.0085;
const float CANVAS_GALLERY_DENSITY = 0.0061;
const float GLASS_DENSITY   = 0.0098;
const float MIRROR_DENSITY  = 0.0191;
const float PATIENT_BOARD_DENSITY  = 0.0347;


// available materials (should match above)
enum MaterialType {
	ACOUSTIC_PANEL,
	ACOUSTIC_PANEL_FRAMED,
	ACRYLIC,
	CANVAS_FRAMED,
	CANVAS_GALLERY,
	GLASS,
    MIRROR,
    PATIENT_BOARD
};


enum GlazingType {
	GLAZING_ACRYLIC,
	GLAZING_GLASS,
	GLAZING_NONE
};

enum HardwareSpec {
    PT_SEC_4,
    PT_SEC_3
};

class Art {
// private variables end with _ to differentiate them (you can only directly use them in Art.cpp, otherwise use the public functions like getLineNumber())
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
    HardwareSpec hardware_;       // TODO if this impacts calculations, change from string

public:
    Art(); // default constructor

    // constructor ("density" isn't in here because you'll set that value based on "material")
    Art(int lineNo, int quantity, std::string tagNo, 
        MaterialType material, float outerWidth, float outerHeight, 
        GlazingType glazeType, std::string frame1Moulding, HardwareSpec hardware);

	// gets values of private instance variables so other files can use them
	int getLineNumber();
    int getQuantity();
    std::string getTagNumber();
    MaterialType getMaterial();
    float getOuterWidth();
    float getOuterHeight();
    GlazingType getGlazeType();
    std::string getFrame1Moulding();
    HardwareSpec getHardware();
    int getWeight() { return 10; }
};
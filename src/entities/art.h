#pragma once

#include <string>

// MATERIAL DENSITY REFERENCE TABLE
// Critical Data - Use these precise densities for ALL calculations

//          Material	 |   Density (LB/SQIN)
const float ACOUSTIC_PANEL_DENSITY = 0.0038;
const float ACOUSTIC_PANEL_FRAMED_DENSITY =	0.0037;
const float CANVAS_FRAMED_GLAZING_ACRYLIC_DENSITY = 0.0094;
const float CANVAS_FRAMED_GLAZING_GLASS_DENSITY   = 0.0098;
const float CANVAS_FRAMED_GLAZING_NONE_DENSITY  = 0.0085;
const float CANVAS_GALLERY_DENSITY = 0.0061;
const float MIRROR_DENSITY  = 0.0191;
const float PATIENT_BOARD_DENSITY  = 0.0347;


// available materials (Glass and Acrylic are ONLY for the glazing of framed canvases, not their own material)
enum MaterialType {
	ACOUSTIC_PANEL,
	ACOUSTIC_PANEL_FRAMED,
	CANVAS_FRAMED,
	CANVAS_GALLERY,
    MIRROR,
    PATIENT_BOARD
};

// Glazing types ONLY apply to framed canvases
enum GlazingType {
	GLAZING_ACRYLIC,
	GLAZING_GLASS,
	GLAZING_NONE
};

enum HardwareSpec {
    PT_SEC_4,
    PT_SEC_3,
    NONE
};

class Art {
// private variables end with _ to differentiate them (you can only directly use them in Art.cpp, otherwise use the public functions like getLineNumber())
private:
    int lineNumber_;
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
    Art(int lineNo, std::string tagNo, 
        MaterialType material, float outerWidth, float outerHeight, 
        GlazingType glazeType, std::string frame1Moulding, HardwareSpec hardware);

	// gets values of private instance variables so other files can use them
	int getLineNumber();
    std::string getTagNumber();
    MaterialType getMaterial();
    float getOuterWidth();
    float getOuterHeight();
    GlazingType getGlazeType();
    std::string getFrame1Moulding();
    HardwareSpec getHardware();
    int getWeight();
    bool needsCustomShipping();
    bool needsCanvasPacking();
};
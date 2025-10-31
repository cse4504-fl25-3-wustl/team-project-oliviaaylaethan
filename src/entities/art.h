#pragma once

#include <string>

// art needs custom packing if dimensions exceed 43.5 x 88 (43.5 x 88 STILL FITS and does NOT need custom packing)
const float LARGE_BOX_LIMIT = 43.5f;
const float CRATE_LIMIT = 46.0f;
const float CUSTOM_PACKING_NEEDED_THRESHOLD_LARGER_DIM = 88.0f; // this is the tallest art can be to still be packable on a pallet

const float OVERSIZED_INSTALLATION_THRESHOLD = 44.0f; // Anything that exceeds (inclusive) 44" in EITHER direction is considered oversized for installation purposes (because needs extra support hardware).

// MATERIAL DENSITY REFERENCE TABLE
// Critical Data - Use these precise densities for ALL calculations

//          Material	 |   Density (LB/SQIN)
const float ACOUSTIC_PANEL_DENSITY = 0.0038;
const float ACOUSTIC_PANEL_FRAMED_DENSITY =	0.0037;
const float CANVAS_FRAMED_DENSITY  = 0.0085;
const float CANVAS_GALLERY_DENSITY = 0.0061;
const float MIRROR_DENSITY  = 0.0191;
const float PAPER_PRINT_GLAZING_ACRYLIC_DENSITY = 0.0094;
const float PAPER_PRINT_GLAZING_GLASS_DENSITY  = 0.0098;
const float PATIENT_BOARD_DENSITY  = 0.0347;

// Pieces per box
const int GLASS_ACRYLIC_FRAMED_PER_BOX = 6;
const int GLASS_ACRYLIC_SUNRISE_PER_BOX = 8;
const int CANVAS_PER_BOX = 6; // TODO: piazza says we can say 6 for now, but test cases used 4
const int ACOUSTIC_PANEL_PER_BOX = 6; // TODO: Piazza says 4
const int MIRROR_PER_BOX = 6;


// available materials (Glass and Acrylic are ONLY for the glazing of framed PAPER PRINTS, not their own material)
enum MaterialType {
	ACOUSTIC_PANEL,
	ACOUSTIC_PANEL_FRAMED,
	CANVAS_FRAMED,
	CANVAS_GALLERY,
    MIRROR,
    PAPER_PRINT_FRAMED,
    PATIENT_BOARD
};

// Glazing types ONLY apply to framed paper prints
enum GlazingType {
	GLAZING_ACRYLIC,
	GLAZING_GLASS,
	GLAZING_NONE
};

enum HardwareSpec {
    PT_SEC_4, // TODO this should apply to art that exceeds OVERSIZED_INSTALLATION_THRESHOLD?
    PT_SEC_3,
    NONE
};

class Art {
private:
    int lineNumber_;
    std::string tagNumber_;
    MaterialType material_;
    float materialDensity_;
    float outerWidth_;
    float outerHeight_;
    GlazingType glazeType_;
    std::string frame1Moulding_; // TODO if this impacts calculations, change from string
    HardwareSpec hardware_;
    int perBoxCount_;

public:
    Art();

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
    bool needsCustomPackaging(float custom_packing_needed_threshold_smaller_dim = LARGE_BOX_LIMIT); // TODO get rid of this default (need to fix test cases function calls for that)
    bool needsCratePacking();
    bool needsLargeCratePacking();
    bool needsCanvasPacking();
    bool isOversizedInstallation();
    int getPerBoxCount();
};
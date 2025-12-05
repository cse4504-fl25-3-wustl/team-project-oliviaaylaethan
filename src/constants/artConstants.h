// ART CONSTANTS
#pragma once

const float OVERSIZED_INSTALLATION_THRESHOLD = 44.0f; // Anything that exceeds (inclusive) 44" in EITHER direction is considered oversized for installation purposes (because needs extra support hardware).

// MATERIAL DENSITY REFERENCE TABLE

//          Material	 |   Density (LB/SQIN)
const float ACOUSTIC_PANEL_DENSITY = 0.0038;
const float ACOUSTIC_PANEL_FRAMED_DENSITY =	0.0037;
const float CANVAS_FRAMED_DENSITY  = 0.0085;
const float CANVAS_GALLERY_DENSITY = 0.0061;
const float MIRROR_DENSITY  = 0.0191;
const float PAPER_PRINT_GLAZING_ACRYLIC_DENSITY = 0.0094;
const float PAPER_PRINT_GLAZING_GLASS_DENSITY  = 0.0098;
const float PATIENT_BOARD_DENSITY  = 0.0347;

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
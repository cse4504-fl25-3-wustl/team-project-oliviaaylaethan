#pragma once
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
// BOX CONSTANTS
#pragma once

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

// Pieces per box
const int GLASS_ACRYLIC_FRAMED_PER_BOX = 6;
const int GLASS_ACRYLIC_SUNRISE_PER_BOX = 8;
const int CANVAS_PER_BOX = 4; // TODO: piazza says we can say 6 for now, but test cases used 4
const int ACOUSTIC_PANEL_PER_BOX = 4; // TODO: Piazza says 4
const int MIRROR_PER_BOX = 6;

// art needs custom packing if dimensions exceed 43.5 x 88 (43.5 x 88 STILL FITS and does NOT need custom packing)
const float LARGE_BOX_LIMIT = 43.5f;
const float CRATE_LIMIT = 46.0f;
const float CUSTOM_PACKING_NEEDED_THRESHOLD_LARGER_DIM = 88.0f; // this is the tallest art can be to still be packable on a pallet

// Rule: As long as at least ONE dimension of an art piece is 36" or less, it will fit in a standard size box. Boxes can be telescoped to a max height of 88"
const int MAX_BOX_HEIGHT = CUSTOM_PACKING_NEEDED_THRESHOLD_LARGER_DIM;
const float STANDARD_BOX_TELESCOPED_THRESHOLD = 36.5f; // Anything that exceeds 36" in ONLY one direction can be telescoped in a standard box.

const bool TODO_PLACEHOLDER_BOOL = false; // for things we don't know the value of yet and need to ask client about for clarification

// Epsilon tolerance for floating point comparisons
constexpr float EPS = 1e-6;
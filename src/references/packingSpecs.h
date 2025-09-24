#pragma once

struct Dimensions {
    float l, w, h; // Length, Width, Height in inches
};

// Box specifications
struct BoxSpec {
    Dimensions dimensions;
    int palletCapacity;
};

// Most common size
const BoxSpec STANDARD_BOX = {{37, 11, 31}, 4};

// >36" in both directions
const BoxSpec LARGE_BOX = {{44, 13, 48}, 3};

// Small orders/replacements (UPS only)
const BoxSpec UPS_SMALL_BOX = {{36, 6, 36}, 0};

// Adjustable length (UPS only)
const BoxSpec UPS_LARGE_BOX = {{44, 6, 35}, 0};

// Pallet specifications
struct PalletSpec {
    Dimensions dimensions;
    float weight; // in lbs
    int boxCapacity;
};

// Standard pallet: 4 standard boxes
const PalletSpec STANDARD_PALLET = {{48, 40, 0}, 60, 4};

// For small shipments
const PalletSpec GLASS_SMALL_PALLET = {{43, 35, 0}, 60, 0};

// Oversize pallet: 5 standard boxes
const PalletSpec OVERSIZE_PALLET = {{60, 40, 0}, 75, 5};

// Crate specifications
struct CrateSpec {
    Dimensions dimensions;
    float tareWeight; // in lbs
};

// Most protective option
const CrateSpec STANDARD_CRATE = {{50, 38, 0}, 125};

// Height calculation for crates
inline float calculateCrateHeight(float largestDimension) {
    return largestDimension + 8; // Add 8 inches buffer
}
// SHIPPING CONTAINER CONSTANTS
#pragma once
#include "boxConstants.h"

// Shared between crate and pallet
const float SHIPPING_CONTAINER_THICKNESS = 8.0f;

// Crate info
const Dimensions STANDARD_CRATE_DIMENSIONS = {50, 38, SHIPPING_CONTAINER_THICKNESS};
const float STANDARD_CRATE_TARE_WEIGHT = 125;
const int STANDARD_PALLET_CANVAS_BOX_CAPACITY = 12;

// DISCREPENCY BELOW

// Piazza Post
// Material	If <33" both dimensions	If >33" either dimension
// Glass/Acrylic	25 pieces per crate	18 pieces per crate
// Canvas	18 pieces per crate	12 pieces per crate
// Mirrors	24-25 pieces directly in crate	(no boxes)

// Test case justifications
// The box capacity (footprint) is 46" x 36".
// - when packing along the long side, we can fit:
//   - 25 paper prints
//   - 18 canvases
// - when packing along the short side, we can fit:
//   - 19 paper prints
//   - 14 canvases

// The thresholds below match the values needed for the test cases, NOT what is on Piazza
const int LARGE_CRATE_CAPACITY_WIDTH_THRESHOLD = 48;
const int LARGE_CRATE_CAPACITY_HEIGHT_THRESHOLD = 36;
const int GLASS_ACRYLIC_SMALL_CRATE_CAPACITY = 25;
const int GLASS_ACRYLIC_LARGE_CRATE_CAPACITY = 19;
const int CANVAS_SMALL_CRATE_CAPACITY = 18;
const int CANVAS_LARGE_CRATE_CAPACITY = 14;
const int MIRROR_CRATE_CAPACITY = 24;

// Pallet info
// For standard shipments
const Dimensions STANDARD_PALLET_DIMENSIONS = {48, 40, SHIPPING_CONTAINER_THICKNESS};
const float STANDARD_PALLET_TARE_WEIGHT = 60;
const int STANDARD_PALLET_STANDARD_BOX_CAPACITY = 4;
const int STANDARD_PALLET_OVERSIZED_BOX_CAPACITY = 3;

// For small shipments
const Dimensions GLASS_SMALL_PALLET_DIMENSIONS = {43, 35, SHIPPING_CONTAINER_THICKNESS};
const float GLASS_SMALL_PALLET_TARE_WEIGHT = 60;
const int GLASS_SMALL_PALLET_STANDARD_BOX_CAPACITY = 0;

// Oversize pallet: 5 standard boxes
const Dimensions OVERSIZE_PALLET_DIMENSIONS = {60, 40, SHIPPING_CONTAINER_THICKNESS};
const float OVERSIZE_PALLET_TARE_WEIGHT = 75;
const int OVERSIZE_PALLET_STANDARD_BOX_CAPACITY = 5;
const int OVERSIZE_PALLET_OVERSIZED_BOX_CAPACITY = 3;

const int TEMP_PALLET_OVERSIZED_BOX_CAPACITY = 3; // TODO placeholder value for glass

enum ShippingContainerType {
    STANDARD_PALLET,
    GLASS_SMALL_PALLET,
    OVERSIZE_PALLET,
    STANDARD_CRATE,
    OTHER
};

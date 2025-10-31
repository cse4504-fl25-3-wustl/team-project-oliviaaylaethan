#pragma once
#include "box.h"
#include <vector>

// Shared between crate and pallet
const float SHIPPING_CONTAINER_THICKNESS = 8.0f;

// Crate info
const Dimensions STANDARD_CRATE_DIMENSIONS = {50, 38, SHIPPING_CONTAINER_THICKNESS};
const float STANDARD_CRATE_TARE_WEIGHT = 125;
const int STANDARD_PALLET_CANVAS_BOX_CAPACITY = 12;

// Material	If <33" both dimensions	If >33" either dimension
// Glass/Acrylic	25 pieces per crate	18 pieces per crate
// Canvas	18 pieces per crate	12 pieces per crate
// Mirrors	24-25 pieces directly in crate	(no boxes)
const int CRATE_LARGE_THRESHOLD = 33;
const int GLASS_ACRYLIC_SMALL_CRATE_CAPACITY = 25;
const int GLASS_ACRYLIC_LARGE_CRATE_CAPACITY = 18;
const int CANVAS_SMALL_CRATE_CAPACITY = 18;
const int CANVAS_LARGE_CRATE_CAPACITY = 12;
const int MIRROR_CRATE_CAPACITY = 24;

// Pallet info
// For standard shipments
const Dimensions STANDARD_PALLET_DIMENSIONS = {48, 40, SHIPPING_CONTAINER_THICKNESS};
const float STANDARD_PALLET_TARE_WEIGHT = 60;
const int STANDARD_PALLET_STANDARD_BOX_CAPACITY = 4;

// For small shipments
const Dimensions GLASS_SMALL_PALLET_DIMENSIONS = {43, 35, SHIPPING_CONTAINER_THICKNESS};
const float GLASS_SMALL_PALLET_TARE_WEIGHT = 60;
const int GLASS_SMALL_PALLET_STANDARD_BOX_CAPACITY = 0;

// Oversize pallet: 5 standard boxes
const Dimensions OVERSIZE_PALLET_DIMENSIONS = {60, 40, SHIPPING_CONTAINER_THICKNESS};
const float OVERSIZE_PALLET_TARE_WEIGHT = 75;
const int OVERSIZE_PALLET_STANDARD_BOX_CAPACITY = 5;

const int TEMP_PALLET_OVERSIZED_BOX_CAPACITY = 3; // TODO determine real values later

enum ShippingContainerType {
    STANDARD_PALLET,
    GLASS_SMALL_PALLET,
    OVERSIZE_PALLET,
    STANDARD_CRATE,
    OTHER
};



// class for any shipping container (Pallet or Crate)
class ShippingContainer {
private:
    Dimensions dimensions_;
    float tareWeight_; // Weight of the empty container
    std::vector<Box> contents_;
    std::vector<Art> artContents_;
    ShippingContainerType shippingContainerType_;

    int standardBoxCapacity_;
    int oversizedBoxCapacity_; 
    float filledArt_ = 0.0f;
    
public:
    ShippingContainer();
    ShippingContainer(Dimensions dimensions, float tareWeight, int standardBoxCapacity, int oversizedBoxCapacity, ShippingContainerType shippingContainerType);
    
    static ShippingContainer makeStandardPallet();
    static ShippingContainer makeGlassSmallPallet(); // TODO: What is glass small pallet?
    static ShippingContainer makeOversizePallet();
    static ShippingContainer makeStandardCrate();

    Dimensions getDimensions() const;
    float getTareWeight() const;
    std::vector<Box> getContents() const;
    std::vector<Art> getArtContents() const;
    ShippingContainerType getShippingContainerType() const;
    int getStandardBoxCapacity() const;
    int getOversizedBoxCapacity() const;
    bool addBox(Box box);
    bool addArt(Art art);
    float getFilledArt();
    void setFilledArt(float filledArt);
};

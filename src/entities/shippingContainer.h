#pragma once
#include "box.h"
#include <vector>

// Crate info
const Dimensions STANDARD_CRATE_DIMENSIONS = {50, 38, 0};
const float STANDARD_CRATE_TARE_WEIGHT = 125;
const int TEMP_CRATE_OVERSIZED_BOX_CAPACITY = 3; // TODO determine real values later
const int TEMP_CRATE_STANDARD_BOX_CAPACITY = 4; // TODO determine real values later


// Pallet info
// For standard shipments
const Dimensions STANDARD_PALLET_DIMENSIONS = {48, 40, 0};
const float STANDARD_PALLET_TARE_WEIGHT = 60;
const int STANDARD_PALLET_STANDARD_BOX_CAPACITY = 4;

// For small shipments
const Dimensions GLASS_SMALL_PALLET_DIMENSIONS = {43, 35, 0};
const float GLASS_SMALL_PALLET_TARE_WEIGHT = 60;
const int GLASS_SMALL_PALLET_STANDARD_BOX_CAPACITY = 0;

// Oversize pallet: 5 standard boxes
const Dimensions OVERSIZE_PALLET_DIMENSIONS = {60, 40, 0};
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
    ShippingContainerType shippingContainerType_;

    int standardBoxCapacity_;
    int oversizedBoxCapacity_; 
    
public:
    ShippingContainer();
    ShippingContainer(Dimensions dimensions, float tareWeight, int standardBoxCapacity, int oversizedBoxCapacity, ShippingContainerType shippingContainerType);
    
    static ShippingContainer makeStandardPallet();
    static ShippingContainer makeGlassSmallPallet(); // TODO: What is glass small pallet?
    static ShippingContainer makeOversizePallet();
    static ShippingContainer makeStandardCrate();

    float calculateContainerHeight();

    Dimensions getDimensions() const;
    float getTareWeight() const;
    std::vector<Box> getContents() const;
    ShippingContainerType getShippingContainerType() const;
    int getStandardBoxCapacity() const;
    int getOversizedBoxCapacity() const;
    void addBox(Box box);
};

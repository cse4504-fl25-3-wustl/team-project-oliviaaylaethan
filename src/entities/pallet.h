#pragma once

#include "shippingContainer.h"
#include <vector>

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

const int TEMP_OVERSIZED_BOX_CAPACITY = 3; // TODO determine real values later

enum PalletType {
    STANDARD_PALLET,
    GLASS_SMALL_PALLET,
    OVERSIZE_PALLET
};

// also includes protected member variables from ShippingContainer
class Pallet : public ShippingContainer {
private:
    int standardBoxCapacity_;
    int oversizedBoxCapacity_; 
    PalletType palletType_;

public:
    Pallet();
    Pallet(Dimensions dimensions, float tareWeight, int standardBoxCapacity, int oversizedBoxCapacity, PalletType palletType);
    
    static Pallet makeStandardPallet();
    static Pallet makeGlassSmallPallet(); // TODO: What is glass small pallet?
    static Pallet makeOversizePallet();
    
    PalletType getPalletType() const;

    // TODO should this have calculatePalletHeight() and add 8 inches the way that crate does?

    // Overridden methods from ShippingContainer
    Dimensions getDimensions() const override;
    float getTareWeight() const override;
    std::vector<Box> getContents() const override;
    void addBox(Box box) override;
};

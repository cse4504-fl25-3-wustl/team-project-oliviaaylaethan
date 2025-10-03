#pragma once

#include "shippingContainer.h"
#include <vector>

const Dimensions STANDARD_CRATE_DIMENSIONS = {50, 38, 0};
const float STANDARD_CRATE_TARE_WEIGHT = 125;


// also includes protected member variables from ShippingContainer
class Crate : public ShippingContainer {
public:
    Crate();
    Crate(Dimensions dimensions, float tareWeight);

    static Crate makeStandardCrate();

    float calculateCrateHeight() const;
    
    // Overridden methods from ShippingContainer
    Dimensions getDimensions() const override;
    float getTareWeight() const override;
    std::vector<Box> getContents() const override;
    void addBox(Box box) override;
};

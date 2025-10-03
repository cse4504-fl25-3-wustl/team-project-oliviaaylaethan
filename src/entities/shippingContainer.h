#pragma once
#include "box.h"
#include <vector>

// Abstract base class for any shipping container (Pallet or Crate)
class ShippingContainer {
protected:
    Dimensions dimensions_;
    float tareWeight_; // Weight of the empty container
    std::vector<Box> contents_;

public:
    virtual ~ShippingContainer() = default;

    // Constructor defined so that pallet and crate can set dimensions and tareWeight
    ShippingContainer(Dimensions dimensions, float tareWeight)
        : dimensions_(dimensions), tareWeight_(tareWeight) {}

    // Pure virtual methods that must be implemented by derived classes
    virtual Dimensions getDimensions() const = 0;
    virtual float getTareWeight() const = 0;
    virtual std::vector<Box> getContents() const = 0;
    virtual void addBox(Box box) = 0;
};

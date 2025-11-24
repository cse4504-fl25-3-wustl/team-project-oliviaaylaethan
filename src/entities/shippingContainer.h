#pragma once
#include "../constants/shippingContainerConstants.h"
#include "box.h"
#include <vector>

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
    float filledFrac_;
    
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
    float getFilledFrac() const;
};

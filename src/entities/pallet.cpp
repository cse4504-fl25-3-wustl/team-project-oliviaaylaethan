#include "pallet.h"

Pallet::Pallet()
    : ShippingContainer({0, 0, 0}, 0.0f),
      standardBoxCapacity_(0),
      oversizedBoxCapacity_(0),
      palletType_(STANDARD_PALLET)
{}

Pallet::Pallet(Dimensions dimensions, float tareWeight, int standardBoxCapacity, int oversizedBoxCapacity, PalletType palletType)
    : ShippingContainer(dimensions, tareWeight),
      standardBoxCapacity_(standardBoxCapacity),
      oversizedBoxCapacity_(oversizedBoxCapacity),
      palletType_(palletType)
{}


Pallet Pallet::makeStandardPallet() {
    return Pallet(
        STANDARD_PALLET_DIMENSIONS,
        STANDARD_PALLET_TARE_WEIGHT,
        STANDARD_PALLET_STANDARD_BOX_CAPACITY,
        TEMP_OVERSIZED_BOX_CAPACITY,
        STANDARD_PALLET
    );
}

Pallet Pallet::makeGlassSmallPallet() {
    return Pallet(
        GLASS_SMALL_PALLET_DIMENSIONS,
        GLASS_SMALL_PALLET_TARE_WEIGHT,
        GLASS_SMALL_PALLET_STANDARD_BOX_CAPACITY,
        TEMP_OVERSIZED_BOX_CAPACITY,
        GLASS_SMALL_PALLET
    );
}

Pallet Pallet::makeOversizePallet() {
    return Pallet(
        OVERSIZE_PALLET_DIMENSIONS,
        OVERSIZE_PALLET_TARE_WEIGHT,
        OVERSIZE_PALLET_STANDARD_BOX_CAPACITY,
        TEMP_OVERSIZED_BOX_CAPACITY,
        OVERSIZE_PALLET
    );
}

PalletType Pallet::getPalletType() const {
    return palletType_;
}

Dimensions Pallet::getDimensions() const {
    return dimensions_;
}

float Pallet::getTareWeight() const {
    return tareWeight_;
}

std::vector<Box> Pallet::getContents() const {
    return contents_;
}

void Pallet::addBox(Box box) {
    contents_.push_back(box);
}
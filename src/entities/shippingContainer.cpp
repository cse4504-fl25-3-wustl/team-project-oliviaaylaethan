#include "shippingContainer.h"

ShippingContainer::ShippingContainer()
    : dimensions_{0, 0, 0},
      tareWeight_(0.0f),
      standardBoxCapacity_(0),
      oversizedBoxCapacity_(0),
      shippingContainerType_(OTHER)
{}

ShippingContainer::ShippingContainer(Dimensions dimensions, float tareWeight, int standardBoxCapacity, int oversizedBoxCapacity, ShippingContainerType shippingContainerType)
    : dimensions_(dimensions),
      tareWeight_(tareWeight),
      standardBoxCapacity_(standardBoxCapacity),
      oversizedBoxCapacity_(oversizedBoxCapacity),
      shippingContainerType_(shippingContainerType)
{}

ShippingContainer ShippingContainer::makeStandardPallet() {
    return ShippingContainer(
        STANDARD_PALLET_DIMENSIONS,
        STANDARD_PALLET_TARE_WEIGHT,
        STANDARD_PALLET_STANDARD_BOX_CAPACITY,
        TEMP_PALLET_OVERSIZED_BOX_CAPACITY,
        STANDARD_PALLET
    );
}

ShippingContainer ShippingContainer::makeGlassSmallPallet() {
    return ShippingContainer(
        GLASS_SMALL_PALLET_DIMENSIONS,
        GLASS_SMALL_PALLET_TARE_WEIGHT,
        GLASS_SMALL_PALLET_STANDARD_BOX_CAPACITY,
        TEMP_PALLET_OVERSIZED_BOX_CAPACITY,
        GLASS_SMALL_PALLET
    );
}

ShippingContainer ShippingContainer::makeOversizePallet() {
    return ShippingContainer(
        OVERSIZE_PALLET_DIMENSIONS,
        OVERSIZE_PALLET_TARE_WEIGHT,
        OVERSIZE_PALLET_STANDARD_BOX_CAPACITY,
        TEMP_PALLET_OVERSIZED_BOX_CAPACITY,
        OVERSIZE_PALLET
    );
}

ShippingContainer ShippingContainer::makeStandardCrate() {
    return ShippingContainer(
        STANDARD_CRATE_DIMENSIONS,
        STANDARD_CRATE_TARE_WEIGHT,
        TEMP_CRATE_STANDARD_BOX_CAPACITY,
        TEMP_CRATE_OVERSIZED_BOX_CAPACITY,
        STANDARD_CRATE
    );
}

// Calculate total container height based on tallest box inside
float ShippingContainer::calculateContainerHeight() {
    float maxBoxHeight = 0.0f;

    for (const Box& box : contents_) {
        float boxHeight = box.getDimensions().h; // FIXME do we know that the h dimension will always be "height" or could a box be turned on its side?
        if (boxHeight > maxBoxHeight) {
            maxBoxHeight = boxHeight;
        }
    }

    return maxBoxHeight + 8.0f; // FIXME is this meant to be 8 inches for CRATES or for PALLETS? Or both?
}

Dimensions ShippingContainer::getDimensions() const {
    return dimensions_;
}

float ShippingContainer::getTareWeight() const {
    return tareWeight_;
}

std::vector<Box> ShippingContainer::getContents() const {
    return contents_;
}

ShippingContainerType ShippingContainer::getShippingContainerType() const {
    return shippingContainerType_;
}

int ShippingContainer::getStandardBoxCapacity() const {
    return standardBoxCapacity_;
}

int ShippingContainer::getOversizedBoxCapacity() const {
    return oversizedBoxCapacity_;
}

void ShippingContainer::addBox(Box box) {
    contents_.push_back(box);     // TODO this used to update totalWeight_, implement that elsewhere
}

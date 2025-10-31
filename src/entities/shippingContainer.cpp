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

Dimensions ShippingContainer::getDimensions() const {
    return dimensions_;
}

float ShippingContainer::getTareWeight() const {
    return tareWeight_;
}

std::vector<Box> ShippingContainer::getContents() const {
    return contents_;
}

std::vector<Art> ShippingContainer::getArtContents() const {
    return artContents_;
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

bool ShippingContainer::addBox(Box box) {
    // won't add the box if container is already full
    // TODO add checks for other types of boxes/pallets once we know what their limits are
    if (shippingContainerType_ == STANDARD_PALLET && box.getBoxType() == STANDARD_BOX && contents_.size() >= STANDARD_PALLET_STANDARD_BOX_CAPACITY) {
        return false;
    }

    contents_.push_back(box);

    // update height of pallet if new box is taller than others
    float maxBoxHeight = 0.0f;
    for (const Box& box : contents_) {
        float boxHeight = box.getDimensions().h;
        if (boxHeight > maxBoxHeight) {
            maxBoxHeight = boxHeight;
        }
    }
    dimensions_.h = maxBoxHeight + SHIPPING_CONTAINER_THICKNESS;

    return true;
}

bool ShippingContainer::addArt(Art art) {
    if (shippingContainerType_ != STANDARD_CRATE) {
        return false; // only crates can have art directly added
    }
    if (artContents_.size() >= TEMP_CRATE_STANDARD_BOX_CAPACITY) {
        return false;
    }
    artContents_.push_back(art);
    return true;
}

#include "crate.h"

Crate::Crate()
    : ShippingContainer({0, 0, 0}, 0.0f)
{}

Crate::Crate(Dimensions dimensions, float tareWeight)
    : ShippingContainer(dimensions, tareWeight)
{}

Crate Crate::makeStandardCrate() {
    return Crate(STANDARD_CRATE_DIMENSIONS, STANDARD_CRATE_TARE_WEIGHT);
}

void Crate::addBox(Box box) {
    contents_.push_back(box);     // TODO this used to update totalWeight_, implement that elsewhere
}

Dimensions Crate::getDimensions() const {
    return dimensions_;
}

float Crate::getTareWeight() const {
    return tareWeight_;
}

std::vector<Box> Crate::getContents() const {
    return contents_;
}

// Calculate total crate height based on tallest box inside
float Crate::calculateCrateHeight() const {
    float maxBoxHeight = 0.0f;

    for (const Box& box : contents_) {
        float boxHeight = box.getDimensions().h; // FIXME do we know that the h dimension will always be "height" or could a box be turned on its side?
        if (boxHeight > maxBoxHeight) {
            maxBoxHeight = boxHeight;
        }
    }

    return maxBoxHeight + 8.0f; // FIXME is this meant to be 8 inches for CRATES or for PALLETS?
}
#include "box.h"

Box::Box(Dimensions dimensions, BoxType boxType) {
    dimensions_ = dimensions;
    boxType_ = boxType;
}

Box Box::makeStandardBox() {
    return Box(STANDARD_BOX_DIMENSIONS, STANDARD_BOX);
}

Box Box::makeLargeBox() {
    return Box(LARGE_BOX_DIMENSIONS, LARGE_BOX);
}

Box Box::makeUPSSmallBox() {
    return Box(UPS_SMALL_BOX_DIMENSIONS, UPS_SMALL_BOX);
}

Box Box::makeUPSLargeBox() {
    return Box(UPS_LARGE_BOX_DIMENSIONS, UPS_LARGE_BOX);
}


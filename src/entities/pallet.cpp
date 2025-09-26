#include "pallet.h"

Pallet::Pallet(Dimensions dimensions, float weight, int standardBoxCapacity, int oversizedBoxCapacity) {
    dimensions_ = dimensions;
    weight_ = weight;
    standardBoxCapacity_ = standardBoxCapacity;
    oversizedBoxCapacity_ = oversizedBoxCapacity;
}

Pallet Pallet::makeStandardPallet() {
    return Pallet(STANDARD_PALLET_DIMENSIONS, STANDARD_PALLET_WEIGHT, STANDARD_PALLET_STANDARD_BOX_CAPACITY, TEMP_OVERSIZED_BOX_CAPACITY);
}

Pallet Pallet::makeGlassSmallPallet() {
    return Pallet(GLASS_SMALL_PALLET_DIMENSIONS, GLASS_SMALL_PALLET_WEIGHT, GLASS_SMALL_PALLET_STANDARD_BOX_CAPACITY, TEMP_OVERSIZED_BOX_CAPACITY);
}

Pallet Pallet::makeOversizePallet() {
    return Pallet(OVERSIZE_PALLET_DIMENSIONS, OVERSIZE_PALLET_WEIGHT, OVERSIZE_PALLET_STANDARD_BOX_CAPACITY, TEMP_OVERSIZED_BOX_CAPACITY);
}

#include "weightCalculator.h"
#include "references/materialsInfo.h"

namespace WeightCalculator {

    float calculateItemWeight(Item item) {
        return item.getOuterWidth() * item.getOuterHeight() * item.getDensity() * item.getQuantity();
    }

    float calculateTotalPalletWeight(int standard, int oversized, int crates) {
        return standard * 60 + oversized * 75 + crates * 125;
    }

    float calculateTotalWeight(list<Item> items, int standard, int oversized, int crates) {
        float sum = calculateTotalPalletWeight(standard, oversized, crates);
        for (Item item : items) {
            sum += calculateItemWeight(item);
        }
        return sum;
    }

}

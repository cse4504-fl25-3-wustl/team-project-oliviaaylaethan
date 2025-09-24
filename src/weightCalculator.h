#pragma once

#include <list>
#include "item.h"

using std::list;

class WeightCalculator {
    private:
        float calculateItemWeight(Item item);
        float calculateTotalPalletWeight(int standard, int oversized, int crates);

    public:
        WeightCalculator() {}
        float calculateTotalWeight(list<Item> items, int standard, int oversized, int crates);
};

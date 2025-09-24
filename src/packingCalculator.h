#pragma once
#include "item.h"
#include "packingSpecs.h"
#include <vector>

class PackingCalculator {
private:
    std::vector<Item> standardItems_; // TBD as to whether this is needed
    std::vector<Item> oversizedItems_;
    std::vector<Item> glassItems_;
    int itemCount_;
    int artworkWeight_;
    int packagingWeight_;
    int finalShipmentWeight_;
public:
    PackingCalculator(std::vector<Item> items);

    // Add an item
    void addItem(Item item);

    // --- Counts ---
    // Boxes
    int getStandardBoxCount();
    int getLargeBoxCount();
    int getTotalBoxCount();

    // Crates
    int getTotalCrateCount();

    // Pallet
    int getPalletCount();

    // Weight
    int getPackagingWeight();


};
#pragma once
#include "item.h"
#include "references/packingSpecs.h"
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

    // --- COUNTS AND WEIGHTS ---
    // Boxes
    int getStandardBoxCount();
    int getLargeBoxCount();
    int getTotalBoxCount();

    // Crates
    int getTotalCrateCount();

    // Weight
    int getPackagingWeight();

    // --- PALLETS ---
    // under requirements portion
    int getStandardPalletCount();
    int getOversizedPalletCount();
    int getTotalPalletCount();

    // under final dimensions portion
    std::vector<PalletSpec> getPalletList();

    // --- HARDWARE CALCULATION ---
    std::string getLineItemHWSummary();
    int getWallHardwareCount();
    int getDrywallAnchorCount();
    int getScrewCount();
    int getTBoltCount();
};
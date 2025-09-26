#include "packingCalculator.h"
#include "references/businessRules.h"
#include "references/materialsInfo.h"
#include "references/packingSpecs.h"
#include "weightCalculator.h"
#include <iostream>
#include <cmath>

PackingCalculator::PackingCalculator(std::vector<Item> items)
    : standardItems_(), oversizedItems_(), glassItems_(), itemCount_(0), artworkWeight_(0), packagingWeight_(0), finalShipmentWeight_(0) {
    for (const auto& item : items) {
        addItem(item);
    }
}

void PackingCalculator::addItem(Item item) {
    // Categorize items based on dimensions and material type
    if (item.getOuterWidth() > 36 || item.getOuterHeight() > 36) {
        oversizedItems_.push_back(item);
    } else {
        standardItems_.push_back(item);
    }

    if (item.getMaterial() == MaterialType::GLASS || item.getMaterial() == MaterialType::MIRROR) {
        glassItems_.push_back(item);
    }

    itemCount_++;
    artworkWeight_ += item.getDensity() * item.getOuterWidth() * item.getOuterHeight();
}

/*
    // Check for critical oversized detection rules
    if (item.getOuterWidth() > 46 || item.getOuterHeight() > 46) {
        std::cout << "⚠️ Item exceeds crate threshold and requires custom pallet." << std::endl;
    } else if (item.getOuterWidth() > 43.5 || item.getOuterHeight() > 43.5) {
        std::cout << "⚠️ Item exceeds large box limit." << std::endl;
    } else if (item.getOuterWidth() > 36 || item.getOuterHeight() > 36) {
        std::cout << "📦 Item requires large box." << std::endl;
    }

    if (item.getOuterHeight() > 102) {
        std::cout << "❌ Item exceeds absolute height limit for LTL shipping." << std::endl;
    } else if (item.getOuterHeight() > 84) {
        std::cout << "⚠️ Item exceeds recommended height limit." << std::endl;
    }
*/

int PackingCalculator::getStandardBoxCount() {
    return std::ceil(static_cast<float>(standardItems_.size()) / STANDARD_BOX.palletCapacity);
}

int PackingCalculator::getLargeBoxCount() {
    return std::ceil(static_cast<float>(oversizedItems_.size()) / LARGE_BOX.palletCapacity);
}

int PackingCalculator::getTotalBoxCount() {
    return getStandardBoxCount() + getLargeBoxCount();
}

int PackingCalculator::getTotalCrateCount() {
    // Use crate rules from businessRules.h
    int glassCrates = std::ceil(static_cast<float>(glassItems_.size()) / 25); // Example: 25 pieces per crate
    return glassCrates;
}

int PackingCalculator::getPackagingWeight() {
    // Add fixed weight for each box and crate
    packagingWeight_ = getTotalBoxCount() * 10 + getTotalCrateCount() * STANDARD_CRATE.tareWeight;
    return packagingWeight_;
}

int PackingCalculator::getStandardPalletCount() {
    return std::ceil(static_cast<float>(getStandardBoxCount()) / STANDARD_PALLET.boxCapacity);
}

int PackingCalculator::getOversizedPalletCount() {
    return std::ceil(static_cast<float>(getLargeBoxCount()) / OVERSIZE_PALLET.boxCapacity);
}

int PackingCalculator::getTotalPalletCount() {
    return getStandardPalletCount() + getOversizedPalletCount();
}

PalletSpec PackingCalculator::getPalletList() {
    // Return the largest pallet spec used
    if (getOversizedPalletCount() > 0) {
        return OVERSIZE_PALLET;
    }
    return STANDARD_PALLET;
}

std::string PackingCalculator::getLineItemHWSummary() {
    // Map to store hardware type and their counts
    std::unordered_map<std::string, int> hardwareSummary;

    // Iterate through all items and accumulate hardware counts
    for (auto& item : standardItems_) {
        std::string hardware = item.getHardware();
        hardwareSummary[hardware]++;
    }
    for (auto& item : oversizedItems_) {
        std::string hardware = item.getHardware();
        hardwareSummary[hardware]++;
    }

    // Build the summary string
    std::string summary = "Hardware Summary: ";
    for (const auto& [hardware, count] : hardwareSummary) {
        summary += hardware + ": " + std::to_string(count) + ", ";
    }

    // Remove the trailing comma and space
    if (!hardwareSummary.empty()) {
        summary.pop_back();
        summary.pop_back();
    }

    return summary;
}

int PackingCalculator::getWallHardwareCount() {
    return getDrywallAnchorCount() + getScrewCount() + getTBoltCount();
}

int PackingCalculator::getDrywallAnchorCount() {
    return itemCount_; // Example: 1 anchor per item
}

int PackingCalculator::getScrewCount() {
    return itemCount_ * 4; // Example: 4 screws per item
}

int PackingCalculator::getTBoltCount() {
    return itemCount_ / 2; // Example: 1 T-bolt per 2 items
}
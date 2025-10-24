#include "response.h"
#include <format>

PalletInfo::PalletInfo(const std::vector<ShippingContainer>& pallets) : pallets_(pallets) {}

int PalletInfo::getStandardPalletCount() {
    int count = 0;
    for (int i = 0; i < getTotalPalletCount(); i++) {
        if (pallets_[i].getShippingContainerType() == STANDARD_PALLET) {
            count++;
        }
    }
    return count;
}

int PalletInfo::getOversizePalletCount() {
    int count = 0;
    for (int i = 0; i < getTotalPalletCount(); i++) {
        if (pallets_[i].getShippingContainerType() == OVERSIZE_PALLET) {
            count++;
        }
    }
    return count;
}

int PalletInfo::getTotalPalletCount() {
    return static_cast<int>(pallets_.size());
}

int PalletInfo::getTotalTareWeight() {
    return getStandardPalletCount() * STANDARD_PALLET_TARE_WEIGHT
        + getOversizePalletCount() * OVERSIZE_PALLET_TARE_WEIGHT;
}

std::string PalletInfo::getPalletWeightSummary() {
    return std::format("- Pallets: {} lbs ({} pallets @ 60-75 lbs each)", getTotalTareWeight(), getTotalPalletCount());
}

std::vector<std::string> PalletInfo::getPalletRequirementsSummary() {
    return {
        std::format("- Standard pallets ({}\"x{}\"): {} pallet",
            STANDARD_PALLET_DIMENSIONS.l,
            STANDARD_PALLET_DIMENSIONS.w,
            getStandardPalletCount()
        ),
        std::format("- Oversize pallets ({}\"x{}\"): {} pallet",
            OVERSIZE_PALLET_DIMENSIONS.l,
            OVERSIZE_PALLET_DIMENSIONS.w,
            getOversizePalletCount()
        )
    };
}

std::vector<std::string> PalletInfo::getPalletDimensionsSummary() {
    int count = getTotalPalletCount();
    std::vector<std::string> summary;
    summary.reserve(count);
    for (int i = 0; i < count; i++) {
        summary.push_back(std::format(
            "{}\"x{}\"x{}\"H @ {} lbs",
            pallets_[i].getDimensions().l,
            pallets_[i].getDimensions().w,
            pallets_[i].getDimensions().h,
            getTotalTareWeight()
        ));
    }
    return summary;
}

std::vector<std::string> PalletInfo::getAllPackedBoxesSummary() {
    int count = getTotalPalletCount();
    if (count < 1) return {};
    std::vector<std::string> summary;
    summary.push_back("\nBox Packing Summary - Pallet:");
    ShippingContainer pallet = ShippingContainer::makeStandardPallet();
    for (int i = 0; i < count; i++) {
        pallet = pallets_[i];
        summary.push_back(std::format(
            "- Pallet {} - {}", i, to_string(pallet.getShippingContainerType())));
        std::vector<Box> contents = pallet.getContents();
        for (size_t j = 0; j < contents.size(); j++) {
            summary.push_back(std::format(
                "  - {}", to_string(contents[j].getBoxType())
            ));
        }
    }
    return summary;
}

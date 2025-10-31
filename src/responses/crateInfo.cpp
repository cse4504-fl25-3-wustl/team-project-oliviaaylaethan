#include "response.h"
#include <format>

CrateInfo::CrateInfo(const std::vector<ShippingContainer>& crates) : crates_(crates) {}

int CrateInfo::getTotalCrateCount() {
    return static_cast<int>(crates_.size());
}

int CrateInfo::getTotalTareWeight() {
    return getTotalCrateCount() * STANDARD_CRATE_TARE_WEIGHT;
}

std::vector<Art> CrateInfo::getAllArt() {
    std::vector<Art> allArt;
    for (ShippingContainer crate : crates_) {
        for (Art art : crate.getArtContents()) {
            allArt.push_back(art);
        }
    }
    return allArt;
}

std::string CrateInfo::getCrateWeightSummary() {
    return std::format("- Crates: {} lbs", getTotalTareWeight());
}

std::string CrateInfo::getCrateRequirementsSummary() {
    return std::format("- Crates: {}", getTotalCrateCount());
}

std::vector<std::string> CrateInfo::getCrateDimensionsSummary() {
    int count = getTotalCrateCount();
    std::vector<std::string> summary;
    summary.reserve(count);
    for (int i = 0; i < count; i++) {
        summary.push_back(std::format(
            "- Crate {}: {}\"x{}\"x{}\"H @ {} lbs",
            i,
            STANDARD_CRATE_DIMENSIONS.l,
            STANDARD_CRATE_DIMENSIONS.w,
            STANDARD_CRATE_DIMENSIONS.h,
            STANDARD_CRATE_TARE_WEIGHT
        ));
    }
    return summary;
}

std::vector<std::string> CrateInfo::getAllPackedBoxesSummary() {
    int count = getTotalCrateCount();
    if (count < 1) return {};
    std::vector<std::string> summary;
    summary.push_back("\nBox Packing Summary - Crate:");
    ShippingContainer crate;
    for (int i = 0; i < count; i++) {
        crate = crates_[i];
        summary.push_back(std::format("- Crate {}", i));
        std::vector<Box> contents = crate.getContents();
        for (size_t j = 0; j < contents.size(); j++) {
            summary.push_back(std::format(
                "  - {}", to_string(contents[j].getBoxType())
            ));
        }
    }
    return summary;
}

#include "response.h"
#include <format>
#include <map>

HardwareInfo::HardwareInfo(const std::vector<Art>& pieces) : pieces_(pieces) {}

int HardwareInfo::getWallHardwareCount() {
    int count = 0;
    for (size_t i = 0; i < pieces_.size(); i++) {
        count += pieces_[i].getQuantity();
    }
    return count;
}

std::vector<std::string> HardwareInfo::getLineItemHWSummary() {
    std::vector<std::string> summary;
    summary.push_back("\nHardware Calculation:");
    summary.push_back("- Line item hardware summary:");

    std::map<HardwareSpec, int> hardwareByType;
    Art art;
    for (size_t i = 0; i < pieces_.size(); i++) {
        art = pieces_[i];
        hardwareByType[art.getHardware()] += art.getQuantity();
    }

    for (const auto& [spec, qty] : hardwareByType) {
        summary.push_back(std::format(
            "  - qty {} pieces with {}",
            qty,
            to_string(spec)
        ));
    }
    return summary;
}

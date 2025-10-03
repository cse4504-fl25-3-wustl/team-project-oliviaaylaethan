#include "response.h"
#include <iostream>

Response::Response(const std::vector<Box>& boxes,
                const std::vector<Pallet>& pallets,
                const std::vector<Crate>& crates) :

    boxInfo_(boxes),
    artInfo_(boxInfo_.getAllArt()),
    crateInfo_(crates),
    palletInfo_(pallets),
    hardwareInfo_(boxInfo_.getAllArt()) {}    

std::vector<std::string> Response::getWeightSummary() {
    std::vector<std::string> summary;
    for (size_t i = 0; i < artInfo_.getTotalWeightSummary().size(); i++) {
        summary.push_back(artInfo_.getTotalWeightSummary()[i]);
    }
    summary.push_back(std::format("\nTotal Packaging Weight: {} lbs",
        palletInfo_.getTotalWeight() + crateInfo_.getTotalWeight()
    ));
    summary.push_back(palletInfo_.getPalletWeightSummary());
    summary.push_back(crateInfo_.getCrateWeightSummary());
    summary.push_back(std::format("\nFinal Shipment Weight: {} lbs",
        artInfo_.getTotalWeight() + palletInfo_.getTotalWeight() + crateInfo_.getTotalWeight()
    ));
    return summary;
}

std::vector<std::vector<std::string>> Response::getPackingSummary() {
    return {
        boxInfo_.getBoxRequirementsSummary(),
        std::vector<std::string>{"\nPallet/Crate Requirements:"},
        palletInfo_.getPalletRequirementsSummary(),
        std::vector<std::string>{crateInfo_.getCrateRequirementsSummary()},
        std::vector<std::string>{"\nFinal Dimensions:"},
        palletInfo_.getPalletDimensionsSummary(),
        crateInfo_.getCrateDimensionsSummary(),
        std::vector<std::string>{"\n-------------------------------"},
        boxInfo_.getAllPackedArtSummary(),
        palletInfo_.getAllPackedBoxesSummary(),
        crateInfo_.getAllPackedBoxesSummary()
    };
}

std::vector<std::string> Response::getBusinessIntelSummary() {
    return {
        artInfo_.getOversizedSummary()
    };
}

std::vector<std::string> Response::getEmailFormatSummary() {
    std::vector<std::string> summary = {"\nShipment Details:"};
    summary.push_back(std::format("- Total Weight: {} lbs", artInfo_.getTotalWeight() + palletInfo_.getTotalWeight() + crateInfo_.getTotalWeight()));
    std::string pieces = "- Pieces:";
    if (palletInfo_.getTotalPalletCount() > 0) {
        pieces.append(std::format(" {} pallets", palletInfo_.getTotalPalletCount()));
    }
    if (crateInfo_.getTotalCrateCount() > 0) {
        pieces.append(std::format(" {} crates", crateInfo_.getTotalCrateCount()));
    }
    return summary;
}

void Response::printWeightSummary() {
    std::vector<std::string> summary = getWeightSummary();
    for (size_t i = 0; i < summary.size(); i++) {
        std::cout << summary[i] << std::endl;
    }
}

void Response::printPackingSummary() {
    std::vector<std::vector<std::string>> summary = getPackingSummary();
    for (size_t i = 0; i < summary.size(); i++) {
        for (size_t j = 0; j < summary[i].size(); j++) {
            std::cout << summary[i][j] << std::endl;
        }
    }
}

void Response::printBusinessIntelSummary() {
    std::vector<std::string> summary = getBusinessIntelSummary();
    for (size_t i = 0; i < summary.size(); i++) {
        std::cout << summary[i] << std::endl;
    }
}

void Response::printEmailFormat() {
    std::vector<std::string> summary = getEmailFormatSummary();
    for (size_t i = 0; i < summary.size(); i++) {
        std::cout << summary[i] << std::endl;
    }
}

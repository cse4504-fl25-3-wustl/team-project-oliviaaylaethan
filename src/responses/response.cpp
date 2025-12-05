#include "response.h"
#include <iostream>
#include <format>

Response::Response(const std::vector<Box>& boxes,
                const std::vector<ShippingContainer>& pallets,
                const std::vector<ShippingContainer>& crates,
                std::shared_ptr<Requirements> requirements,
                const std::vector<Art>& csvArt) :

    boxInfo_(boxes),
    crateInfo_(crates),
    palletInfo_(pallets),
    requirements_(std::move(requirements)),
    allArt_(csvArt)
    {
        artInfo_ = ArtInfo(allArt_, requirements_);
        hardwareInfo_ = HardwareInfo(allArt_);
    }

std::vector<std::string> Response::getWeightSummary() {
    std::vector<std::string> summary = {"\n\n--------- WEIGHT SUMMARY ---------"};
    for (size_t i = 0; i < artInfo_.getTotalWeightSummary().size(); i++) {
        summary.push_back(artInfo_.getTotalWeightSummary()[i]);
    }
    summary.push_back(std::format("\nTotal Packaging Weight: {} lbs",
        palletInfo_.getTotalTareWeight() + crateInfo_.getTotalTareWeight()
    ));
    summary.push_back(palletInfo_.getPalletWeightSummary());
    summary.push_back(crateInfo_.getCrateWeightSummary());
    summary.push_back(std::format("\nFinal Shipment Weight: {} lbs",
        artInfo_.getTotalWeight() + palletInfo_.getTotalTareWeight() + crateInfo_.getTotalTareWeight()
    ));
    return summary;
}

std::vector<std::vector<std::string>> Response::getPackingSummary() {
    std::vector<std::vector<std::string>> summary = {
        std::vector<std::string>{"\n\n--------- PACKING SUMMARY ---------"},
        boxInfo_.getBoxRequirementsSummary(),
        std::vector<std::string>{"\nPallet/Crate Requirements:"},
        palletInfo_.getPalletRequirementsSummary(),
        std::vector<std::string>{crateInfo_.getCrateRequirementsSummary()},
        std::vector<std::string>{"\nFinal Dimensions:"}
    };
    std::vector<std::string> palletDimens = {};
    for (size_t i = 0; i < palletInfo_.getTotalPalletCount(); i++) {
        palletDimens.push_back(std::format("- Pallet {}: {}",
            // index from 1 in print-out
            i+1, palletInfo_.getPalletDimensionsSummary()[i]));
    }
    summary.push_back(palletDimens);
    summary.push_back(crateInfo_.getCrateDimensionsSummary());
    summary.push_back(hardwareInfo_.getLineItemHWSummary());
    summary.push_back(std::vector<std::string>{"\n-----------------------------------"});
    summary.push_back(boxInfo_.getAllPackedArtSummary());
    summary.push_back(palletInfo_.getAllPackedBoxesSummary());
    summary.push_back(crateInfo_.getAllPackedBoxesSummary());
    return summary;
}

std::vector<std::string> Response::getBusinessIntelSummary() {
    std::vector<std::string> summary = {"\n\n--------- BUSINESS INTEL SUMMARY ---------"};
    for (size_t i = 0; i < artInfo_.getOversizedSummary().size(); i++) {
        summary.push_back(artInfo_.getOversizedSummary()[i]);
    }
    summary.push_back("\n");
    for (size_t i = 0; i < artInfo_.getCustomSummary().size(); i++) {
        summary.push_back(artInfo_.getCustomSummary()[i]);
    }
    return summary;
}

std::vector<std::string> Response::getEmailFormatSummary() {
    std::vector<std::string> summary = {"\n\n--------- EMAIL FORMAT ---------"};
    // summary.push_back(std::format("Subject: Quote Request - {}", requirements_->getClientName()));
    summary.push_back("Subject: Quote Request");

    summary.push_back("\nShipment Details:");
    summary.push_back(std::format("- Total Weight: {} lbs", artInfo_.getTotalWeight() + palletInfo_.getTotalTareWeight() + crateInfo_.getTotalTareWeight()));

    std::string pieces = "- Pieces:";
    if (palletInfo_.getTotalPalletCount() > 0) {
        pieces.append(std::format(" {} pallets", palletInfo_.getTotalPalletCount()));
        if (crateInfo_.getTotalCrateCount() > 0) {
            pieces.append(",");
        }
    }
    if (crateInfo_.getTotalCrateCount() > 0) {
        pieces.append(std::format(" {} crates", crateInfo_.getTotalCrateCount()));
    }
    summary.push_back(pieces);
    std::string dimensions = "- Dimensions: ";
    for (size_t i = 0; i < palletInfo_.getTotalPalletCount(); i++) {
        dimensions.append(palletInfo_.getPalletDimensionsSummary()[i]);
        
        if (i+1 < palletInfo_.getTotalPalletCount()) {
            dimensions.append(", ");
        }
    }
    summary.push_back(dimensions);

    summary.push_back("- Pickup: ARCH Design, St. Louis, MO");
    summary.push_back(std::format("- Delivery: {}", requirements_->getJobSiteLocation()));

    std::string specialReq = "- Special Requirements: ";
    if (requirements_->getHasLoadingDock()) {
        specialReq.append("Has loading dock, ");
    }
    if (requirements_->getNeedsLiftgate()) {
        specialReq.append("Needs liftgate, ");
    }
    if (requirements_->getNeedsInsideDelivery()) {
        specialReq.append("Needs inside delivery");
    }
    summary.push_back(specialReq);
    
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
    std::cout << "\n" << std::endl;
}

BoxInfo Response::getBoxInfo() {
    return boxInfo_;
}

ArtInfo Response::getArtInfo() {
    return artInfo_;
}

CrateInfo Response::getCrateInfo() {
    return crateInfo_;
}

PalletInfo Response::getPalletInfo() {
    return palletInfo_;
}

HardwareInfo Response::getHardwareInfo() {
    return hardwareInfo_;
}

Requirements Response::getRequirements() {
    return *requirements_;
}

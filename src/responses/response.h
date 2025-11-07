#pragma once
#include <iostream>
#include <map>
#include "../entities/box.h"
#include "../entities/art.h"
#include "../entities/shippingContainer.h"
#include "../entities/requirements.h"

class BoxInfo {
    private:
        std::vector<Box> boxes_;
    public:
        BoxInfo(const std::vector<Box>& boxes);
        int getStandardBoxCount();
        int getLargeBoxCount();
        int getTotalBoxCount();
        std::vector<std::string> getBoxRequirementsSummary();
        int getTotalWeight();
        std::vector<std::string> getAllPackedArtSummary();
        std::vector<Art> getAllArt();
};

class ArtInfo {
    private:
        // List of all art pieces in order
        std::vector<Art> pieces_;

        // Map of quantity of each line number
        std::map<int, int> quantities_;

        // Map of Art pieces that represent each line number
        // NEW: WILL REPRESENT EACH UNIQUE TYPE OF ART (with uniqueArtId_)
        std::map<int, Art> artTypes_;

        Requirements* requirements_; // pointer, no ownership
    public:
        ArtInfo() {}
        ArtInfo(const std::vector<Art>& pieces, Requirements* req);
        int getTotalCount();
        int getStandardCount();
        int getOversizedCount();
        int getCustomCount();
        int getQuantity(int lineNo);
        Art getArtType(int lineNo);
        std::vector<std::string> getOversizedSummary();
        std::vector<Art> getOversizedItems();
        std::vector<std::string> getTotalWeightSummary();
        int getTotalWeight();
};




































class CrateInfo {
    private:
        std::vector<ShippingContainer> crates_;
    public:
        CrateInfo(const std::vector<ShippingContainer>& crates);
        std::vector<Art> getAllArt();
        int getTotalCrateCount();
        int getTotalTareWeight();
        std::string getCrateWeightSummary();
        std::string getCrateRequirementsSummary();
        std::vector<std::string> getCrateDimensionsSummary();
        std::vector<std::string> getAllPackedBoxesSummary();
};

class PalletInfo {
    private:
        std::vector<ShippingContainer> pallets_;
    public:
        PalletInfo(const std::vector<ShippingContainer>& pallets);
        int getStandardPalletCount();
        int getOversizePalletCount();
        int getTotalPalletCount();
        int getTotalTareWeight();
        std::string getPalletWeightSummary();
        std::vector<std::string> getPalletRequirementsSummary();
        std::vector<std::string> getPalletDimensionsSummary();
        std::vector<std::string> getAllPackedBoxesSummary();
};

class HardwareInfo {
    private:
        std::vector<Art> pieces_;
    public:
        HardwareInfo() {};
        HardwareInfo(const std::vector<Art>& pieces);
        std::vector<std::string> getLineItemHWSummary();
        int getWallHardwareCount();

        // TODO below
        // int getDrywallAnchorCount();
        // int getScrewCount();
        // int getTBoltCount();
};

inline std::string to_string(BoxType type) {
    switch (type) {
        case STANDARD_BOX: return "Standard Box";
        case LARGE_BOX: return "Large Box";
        default: return "Unknown Box";
    }
}

inline std::string to_string(ShippingContainerType type) {
    switch (type) {
        case STANDARD_PALLET: return "Standard Pallet";
        case GLASS_SMALL_PALLET: return "Glass Small Pallet";
        case OVERSIZE_PALLET: return "Oversize Pallet";
        case STANDARD_CRATE: return "Standard Crate";
        case OTHER: return "Other Shipping Container";
        default: return "Unknown";
    }
}

inline std::string to_string(HardwareSpec hardware) {
    switch (hardware) {
        case PT_SEC_4: return "4 pt Sec";
        case PT_SEC_3: return "3 pt Sec";
        default: return "Unknown";
    }
}

class Response {
    private:
        BoxInfo boxInfo_;
        ArtInfo artInfo_;
        CrateInfo crateInfo_;
        PalletInfo palletInfo_;
        HardwareInfo hardwareInfo_;
        Requirements requirements_;
        std::vector<Art> allArt_;

    public:
        Response(const std::vector<Box>& boxes,
                const std::vector<ShippingContainer>& pallets,
                const std::vector<ShippingContainer>& crates,
                const Requirements requirements,
                const std::vector<Art>& allArt);

        std::vector<std::string> getWeightSummary();
        std::vector<std::vector<std::string>> getPackingSummary();
        std::vector<std::string> getBusinessIntelSummary();
        std::vector<std::string> getEmailFormatSummary();

        void printWeightSummary();
        void printPackingSummary();
        void printBusinessIntelSummary();
        void printEmailFormat();

        BoxInfo getBoxInfo();
        ArtInfo getArtInfo();
        CrateInfo getCrateInfo();
        PalletInfo getPalletInfo();
        HardwareInfo getHardwareInfo();
        Requirements getRequirements();
};

#pragma once
#include <iostream>
#include "../entities/box.h"
#include "../entities/art.h"
#include "../entities/crate.h"
#include "../entities/pallet.h"

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
        std::vector<Art> pieces_;
    public:
        ArtInfo(const std::vector<Art>& pieces);
        int getTotalCount();
        int getStandardCount();
        int getOversizedCount();
        std::vector<std::string> getOversizedSummary();
        std::vector<std::string> getTotalWeightSummary();
        int getTotalWeight();
};

class CrateInfo {
    private:
        std::vector<Crate> crates_;
    public:
        CrateInfo(const std::vector<Crate>& crates);
        int getTotalCrateCount();
        int getTotalWeight();
        std::string getCrateWeightSummary();
        std::string getCrateRequirementsSummary();
        std::vector<std::string> getCrateDimensionsSummary();
        std::vector<std::string> getAllPackedBoxesSummary();
};

class PalletInfo {
    private:
        std::vector<Pallet> pallets_;
    public:
        PalletInfo(const std::vector<Pallet>& pallets);
        int getStandardPalletCount();
        int getOversizePalletCount();
        int getTotalPalletCount();
        int getTotalWeight();
        std::string getPalletWeightSummary();
        std::vector<std::string> getPalletRequirementsSummary();
        std::vector<std::string> getPalletDimensionsSummary();
        std::vector<std::string> getAllPackedBoxesSummary();
};

class HardwareInfo {
    private:
        std::vector<Art> pieces_;
    public:
        HardwareInfo(const std::vector<Art>& pieces) : pieces_(pieces) {};
        std::vector<std::string> getLineItemHWSummary();
        int getWallHardwareCount();

        // TODO below
        // int getDrywallAnchorCount();
        // int getScrewCount();
        // int getTBoltCount();
};

inline std::string to_string(BoxType type) {
    switch (type) {
        case STANDARD_BOX: return "Standard";
        case LARGE_BOX: return "Large";
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

    public:
        Response(const std::vector<Box>& boxes,
                const std::vector<Pallet>& pallets,
                const std::vector<Crate>& crates);

        std::vector<std::string> getWeightSummary();
        std::vector<std::vector<std::string>> getPackingSummary();
        std::vector<std::string> getBusinessIntelSummary();
        std::vector<std::string> getEmailFormatSummary();

        void printWeightSummary();
        void printPackingSummary();
        void printBusinessIntelSummary();
        void printEmailFormat();
};

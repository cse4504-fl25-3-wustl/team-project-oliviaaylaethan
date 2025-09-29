#pragma once
#include "../entities/box.h"
#include <iostream>

class BoxInfo {
    public:
        int getStandardBoxCount();
        int getLargeBoxCount();
        int getTotalBoxCount();
};

class ArtInfo {
    public:
        int getTotalCount();
        int getStandardCount();
        int getOversizedCount();
        std::string getOversizedSummary();
        int getTotalWeight();
};

class CrateInfo {
    public:
        int getTotalCrateCount();
        int getTotalWeight();
};

class PalletInfo {
    public:
        int getStandardPalletCount();
        int getOversizedPalletCount();
        int getTotalPalletCount();
        int getTotalWeight();
};

class HardwareInfo {
    public:
        std::string getLineItemHWSummary();
        int getWallHardwareCount();
        int getDrywallAnchorCount();
        int getScrewCount();
        int getTBoltCount();
};

class Response {
    public:
        Response() {};
        void printWeightSummary() {
            std::cout << "Work Order Summary:" << std::endl;
            std::cout << "- Total Pieces: 55" << std::endl;
            std::cout << "- Standard Size Pieces: 49 (estimated at 43\" x 33\")" << std::endl;
            std::cout << "- Oversized Pieces: 6" << std::endl;
            std::cout << "  * 46\" x 34\" (Qty: 2) = 31 lbs" << std::endl;
            std::cout << "  * 56\" x 32\" (Qty: 1) = 18 lbs" << std::endl;
            std::cout << "  * 48\" x 32\" (Qty: 3) = 45 lbs\n" << std::endl;
            std::cout << "Total Artwork Weight: 778 lbs" << std::endl;
            std::cout << "Total Packaging Weight: 135 lbs" << std::endl;
            std::cout << "Final Shipment Weight: 913 lbs" << std::endl;
        };

        void printPackingSummary() {
            std::cout << "Box Requirements:" << std::endl;
            std::cout << "- Standard boxes (37\"×11\"×31\"): 7 boxes" << std::endl;
            std::cout << "- Large boxes (44\"×13\"×48\"): 2 boxes" << std::endl;
            std::cout << "- Total boxes: 9\n" << std::endl;

            std::cout << "Pallet/Crate Requirements:" << std::endl;
            std::cout << "- Standard pallets (48\"×40\"): 1 pallet" << std::endl;
            std::cout << "- Oversize pallets (60\"×40\"): 1 pallet" << std::endl;
            std::cout << "- Crates: 0\n" << std::endl;

            std::cout << "Final Dimensions:" << std::endl;
            std::cout << "- Pallet 1: 48\"×40\"×64\"H @ 450 lbs" << std::endl;
            std::cout << "- Pallet 2: 60\"×40\"×52\"H @ 463 lbs\n" << std::endl;

            std::cout << "Hardware Calculation:" << std::endl;
            std::cout << "- Line item hardware summary; EX qty 44 pieces with 3 pt Sec, qty 12 pieces with 4 pt Sec, etc." << std::endl;
            std::cout << "- Wall hardware needed: 165 pieces" << std::endl;
            std::cout << "  * Drywall anchors: 55" << std::endl;
            std::cout << "  * Screws: 110" << std::endl;
            std::cout << "  * T-bolts: 0\n" << std::endl;

            std::cout << "Packing Order:" << std::endl;
            std::cout << "1 -> Box 1 -> Pallet 1" << std::endl;
            std::cout << "2 -> Box 1 -> Pallet 1" << std::endl;
        };

        void printBusinessIntelSummary() {
            std::cout << "Client-Specific Rules Applied:" << std::endl;
            std::cout << "- Standard packing (no client restrictions)\n" << std::endl;

            std::cout << "Oversized Items Flagged:" << std::endl;
            std::cout << "- 46\"×34\" (Qty: 2) - Requires large box" << std::endl;
            std::cout << "- 56\"×32\" (Qty: 1) - Requires large box" << std::endl;
            std::cout << "- 48\"×32\" (Qty: 3) - Requires large box\n" << std::endl;

            std::cout << "Special Final Mediums to Flag:" << std::endl;
            std::cout << "- Wall Decor - usually gets URL of the item (we will plan to unclude the URL from our system)" << std::endl;
            std::cout << "- Commissions (these are usually high value pieces)\n" << std::endl;


            std::cout << "Alternative Recommendations:" << std::endl;
            std::cout << "- Current method: Pallets only" << std::endl;
            std::cout << "- Alternative: Mix of crates + pallets (may reduce cost)" << std::endl;
            std::cout << "- Cost difference: ~$50 savings with crate option\n" << std::endl;

            std::cout << "Risk Flags:" << std::endl;
            std::cout << "- No high-risk items detected, Glass, Mirrors or breakable or Fragile items." << std::endl;
        };

        void printEmailFormat() {
            std::cout << "Subject: Quote Request - WO 21234 - OLG Ortho Sports\n" << std::endl;

            std::cout << "Shipment Details:" << std::endl;
            std::cout << "- Total Weight: 913 lbs" << std::endl;
            std::cout << "- Pieces: 2 pallets" << std::endl;
            std::cout << "- Dimensions: 48\"×40\"×64\" @ 450 lbs, 60\"×40\"×52\" @ 463 lbs" << std::endl;
            std::cout << "- Pickup: ARCH Design, St. Louis, MO" << std::endl;
            std::cout << "- Delivery: [Site Address]" << std::endl;
            std::cout << "- Special Requirements: [Loading dock, liftgate, etc.]" << std::endl;
        };
};

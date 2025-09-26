class BoxInfo {

};

class ArtInfo {
public:
       
};

 class CrateInfo {

 };


 
 // Boxes
        int getStandardBoxCount();
        int getLargeBoxCount();
        int getTotalBoxCount();

        // Crates
        int getTotalCrateCount();

        // Pallets
        int getStandardPalletCount();
        int getOversizedPalletCount();
        int getTotalPalletCount();

        // Hardware
        std::string getLineItemHWSummary();
        int getWallHardwareCount();
        int getDrywallAnchorCount();
        int getScrewCount();
        int getTBoltCount();
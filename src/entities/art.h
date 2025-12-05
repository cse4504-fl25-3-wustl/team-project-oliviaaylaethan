#pragma once

#include "../constants/artConstants.h"
#include "../constants/boxConstants.h"
#include <string>

class Art {
private:
    int lineNumber_;
    std::string tagNumber_;
    std::string raw_csv_input_material_; // to use for the printed packing summaries
    MaterialType material_;
    float materialDensity_;
    float outerWidth_;
    float outerHeight_;
    GlazingType glazeType_;
    std::string frame1Moulding_; // TODO if this impacts calculations, change from string
    HardwareSpec hardware_;
    int perBoxCount_;
    int perCrateCount_;

    int uniqueArtId_; // mimics lineNumber_ for separating unique types of art if csv has multiple lines listed with same line number    

    void setMaterialDensityAndCounts();

public:
    Art();

    // constructor ("density" isn't in here because you'll set that value based on "material")
    Art(int lineNo, std::string tagNo, 
        MaterialType material, float outerWidth, float outerHeight, 
        GlazingType glazeType, std::string frame1Moulding, HardwareSpec hardware);

    // constructor with uniqueID workaround for badly-formatted input csv files
    Art(int lineNo, std::string tagNo, 
        MaterialType material, float outerWidth, float outerHeight, 
        GlazingType glazeType, std::string frame1Moulding, HardwareSpec hardware, int artID);

	// gets values of private instance variables so other files can use them
    int getUniqueID();
	int getLineNumber();
    std::string getTagNumber();
    void setRawCSVInputMaterial(std::string finalMedium);
    std::string getRawCSVInputMaterial();
    MaterialType getMaterial();
    float getDepth();
    float getOuterWidth();
    float getOuterHeight();
    GlazingType getGlazeType();
    std::string getFrame1Moulding();
    HardwareSpec getHardware();
    int getWeight();
    bool needsCustomPackaging(float custom_packing_needed_threshold_smaller_dim = LARGE_BOX_LIMIT); // TODO get rid of this default (need to fix test cases function calls for that)
    bool needsCratePacking();
    bool needsCanvasPacking();
    bool isOversizedInstallation();
    bool needsLargeCrateCapacity();
    int getPerBoxCount();
    int getPerCrateCount();
};
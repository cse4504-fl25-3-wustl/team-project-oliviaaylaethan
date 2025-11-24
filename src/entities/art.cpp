#include "art.h"
#include "shippingContainer.h"
#include <cmath>

// Default constructor
Art::Art() : lineNumber_(0), tagNumber_(""), material_(MaterialType::CANVAS_FRAMED),
             materialDensity_(0.0f), outerWidth_(0.0f), outerHeight_(0.0f), glazeType_(GlazingType::GLAZING_NONE),
             frame1Moulding_(""), hardware_(HardwareSpec::NONE), perBoxCount_(0), perCrateCount_(0), uniqueArtId_(0) {}

// Constructor to initialize the Art object (assuming properly formatted input csv)
Art::Art(int lineNo, std::string tagNo, 
           MaterialType material, float outerWidth, float outerHeight, 
           GlazingType glazeType, std::string frame1Moulding, HardwareSpec hardware)
    : lineNumber_(lineNo), 
      tagNumber_(tagNo), 
      material_(material), 
      outerWidth_(outerWidth), 
      outerHeight_(outerHeight), 
      glazeType_(glazeType), 
      frame1Moulding_(frame1Moulding), 
      hardware_(hardware),
      uniqueArtId_(-1)
{
    setMaterialDensityAndCounts();    
}




// Constructor to initialize the Art object (with uniqueID)
Art::Art(int lineNo, std::string tagNo, 
           MaterialType material, float outerWidth, float outerHeight, 
           GlazingType glazeType, std::string frame1Moulding, HardwareSpec hardware, int uniqueID)
    : lineNumber_(lineNo), 
      tagNumber_(tagNo), 
      material_(material), 
      outerWidth_(outerWidth), 
      outerHeight_(outerHeight), 
      glazeType_(glazeType), 
      frame1Moulding_(frame1Moulding), 
      hardware_(hardware),
      uniqueArtId_(uniqueID)
{
    setMaterialDensityAndCounts();
}

void Art::setMaterialDensityAndCounts() {
    switch (material_) {
        case MaterialType::ACOUSTIC_PANEL:
            materialDensity_ = ACOUSTIC_PANEL_DENSITY;
            perBoxCount_ = ACOUSTIC_PANEL_PER_BOX;
            perCrateCount_ = needsLargeCrateCapacity() ? CANVAS_LARGE_CRATE_CAPACITY : CANVAS_SMALL_CRATE_CAPACITY;
            break;
        case MaterialType::ACOUSTIC_PANEL_FRAMED:
            materialDensity_ = ACOUSTIC_PANEL_FRAMED_DENSITY;
            perBoxCount_ = ACOUSTIC_PANEL_PER_BOX;
            perCrateCount_ = needsLargeCrateCapacity() ? CANVAS_LARGE_CRATE_CAPACITY : CANVAS_SMALL_CRATE_CAPACITY;
            break;
        case MaterialType::CANVAS_FRAMED:
            materialDensity_ = CANVAS_FRAMED_DENSITY;
            perBoxCount_ = CANVAS_PER_BOX;
            perCrateCount_ = needsLargeCrateCapacity() ? CANVAS_LARGE_CRATE_CAPACITY : CANVAS_SMALL_CRATE_CAPACITY;
            break;
        case MaterialType::CANVAS_GALLERY:
            materialDensity_ = CANVAS_GALLERY_DENSITY;
            perBoxCount_ = CANVAS_PER_BOX;
            perCrateCount_ = needsLargeCrateCapacity() ? CANVAS_LARGE_CRATE_CAPACITY : CANVAS_SMALL_CRATE_CAPACITY;
            break;
        case MaterialType::MIRROR:
            materialDensity_ = MIRROR_DENSITY;
            perBoxCount_ = MIRROR_PER_BOX;
            perCrateCount_ = MIRROR_CRATE_CAPACITY;
            break;
        case MaterialType::PAPER_PRINT_FRAMED:
            if(glazeType_ == GlazingType::GLAZING_ACRYLIC) {
                materialDensity_ = PAPER_PRINT_GLAZING_ACRYLIC_DENSITY;
                perBoxCount_ = GLASS_ACRYLIC_FRAMED_PER_BOX;
                perCrateCount_ = needsLargeCrateCapacity() ? GLASS_ACRYLIC_LARGE_CRATE_CAPACITY : GLASS_ACRYLIC_SMALL_CRATE_CAPACITY;
            }
            else if(glazeType_ == GlazingType::GLAZING_GLASS) {
                materialDensity_ = PAPER_PRINT_GLAZING_GLASS_DENSITY;
                perBoxCount_ = GLASS_ACRYLIC_FRAMED_PER_BOX;
                perCrateCount_ = needsLargeCrateCapacity() ? GLASS_ACRYLIC_LARGE_CRATE_CAPACITY : GLASS_ACRYLIC_SMALL_CRATE_CAPACITY;
            }
            else {
                materialDensity_ = 0.0f;  // Default to 0 since we weren't given a density for glazeless paper prints
                perBoxCount_ = GLASS_ACRYLIC_SUNRISE_PER_BOX;
                perCrateCount_ = GLASS_ACRYLIC_SMALL_CRATE_CAPACITY; // for now since unspecified
            }
            break;
        case MaterialType::PATIENT_BOARD:
            materialDensity_ = PATIENT_BOARD_DENSITY;
            perBoxCount_ = 0; // for now since unspecified
            perCrateCount_ = 0;
            break;
        default:
            materialDensity_ = 0.0f;  // Default to 0 if the material is unknown
            perBoxCount_ = 0;
            perCrateCount_ = 0;
            break;
    }
}

// Getter functions to access private member variables

int Art::getUniqueID() {
    return uniqueArtId_;
}

int Art::getLineNumber() {
    return lineNumber_;
}

std::string Art::getTagNumber() {
    return tagNumber_;
}

MaterialType Art::getMaterial() {
    return material_;
}

float Art::getDepth() {
    if (getPerBoxCount() > 0) {
        return 1.0f / getPerBoxCount() * STANDARD_BOX_DIMENSIONS.w;
    } else {
        return 0.0f;
    }
}

float Art::getOuterWidth() {
    return outerWidth_;
}

float Art::getOuterHeight() {
    return outerHeight_;
}

GlazingType Art::getGlazeType() {
    return glazeType_;
}

std::string Art::getFrame1Moulding() {
    return frame1Moulding_;
}

HardwareSpec Art::getHardware() {
    return hardware_;
}

int Art::getWeight() {
    return std::ceil(outerWidth_ * outerHeight_ * materialDensity_);
}

int Art::getPerBoxCount() {
    return perBoxCount_;
}

bool Art::needsCustomPackaging(const float custom_packing_needed_threshold_smaller_dim) { // by default assumes the client does NOT accept crates
    // for PALLET packaging:
        // art needs custom packing if dimensions exceed 43.5 x 88 (43.5 x 88 STILL FITS and does NOT need custom packing)
        // --> if any dimension exceeds 88, needs custom
        // --> if both dimensions exceed 43.5, needs custom
        // --> if only ONE dimension exceeds 43.5, do NOT need custom
    // for CRATE packaging:
        // same rules as above but replace 43.5 with 46
    if ((outerWidth_ > CUSTOM_PACKING_NEEDED_THRESHOLD_LARGER_DIM || outerHeight_ > CUSTOM_PACKING_NEEDED_THRESHOLD_LARGER_DIM)
        || (outerWidth_ > custom_packing_needed_threshold_smaller_dim && outerHeight_ > custom_packing_needed_threshold_smaller_dim)) {
        return true;
    }
    return false;
}

// Crate Threshold: >46"
bool Art::needsCratePacking() {
    return (outerWidth_ > CRATE_LIMIT || outerHeight_ > CRATE_LIMIT) && !needsCustomPackaging(CRATE_LIMIT);
}

bool Art::needsCanvasPacking() {
    return material_ == MaterialType::CANVAS_FRAMED || material_ == MaterialType::CANVAS_GALLERY;
}

bool Art::isOversizedInstallation() {
    if (outerWidth_ >= OVERSIZED_INSTALLATION_THRESHOLD || outerHeight_ >= OVERSIZED_INSTALLATION_THRESHOLD) {
        return true;
    }
    return false;
}

bool Art::needsLargeCrateCapacity() {
    return !((outerWidth_ <= LARGE_CRATE_CAPACITY_WIDTH_THRESHOLD && outerHeight_ <= LARGE_CRATE_CAPACITY_HEIGHT_THRESHOLD) ||
         (outerWidth_ <= LARGE_CRATE_CAPACITY_HEIGHT_THRESHOLD && outerHeight_ <= LARGE_CRATE_CAPACITY_WIDTH_THRESHOLD));

}

int Art::getPerCrateCount() {
    return perCrateCount_;
}

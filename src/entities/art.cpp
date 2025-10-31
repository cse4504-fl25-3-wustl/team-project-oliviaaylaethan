#include "art.h"
#include <cmath>

// Default constructor
Art::Art() : lineNumber_(0), tagNumber_(""), material_(MaterialType::CANVAS_FRAMED),
             materialDensity_(0.0f), outerWidth_(0.0f), outerHeight_(0.0f), glazeType_(GlazingType::GLAZING_NONE),
             frame1Moulding_(""), hardware_(HardwareSpec::NONE) {}

// Constructor to initialize the Art object
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
      hardware_(hardware)
{
    // Set the materialDensity_ based on the material
    switch (material_) {
        case MaterialType::ACOUSTIC_PANEL:
            materialDensity_ = ACOUSTIC_PANEL_DENSITY;
            perBoxCount_ = ACOUSTIC_PANEL_PER_BOX;
            break;
        case MaterialType::ACOUSTIC_PANEL_FRAMED:
            materialDensity_ = ACOUSTIC_PANEL_FRAMED_DENSITY;
            perBoxCount_ = ACOUSTIC_PANEL_PER_BOX;
            break;
        case MaterialType::CANVAS_FRAMED:
            materialDensity_ = CANVAS_FRAMED_DENSITY;
            perBoxCount_ = CANVAS_PER_BOX;
            break;
        case MaterialType::CANVAS_GALLERY:
            materialDensity_ = CANVAS_GALLERY_DENSITY;
            perBoxCount_ = CANVAS_PER_BOX;
            break;
        case MaterialType::MIRROR:
            materialDensity_ = MIRROR_DENSITY;
            perBoxCount_ = MIRROR_PER_BOX;
            break;
        case MaterialType::PAPER_PRINT_FRAMED:
            if(glazeType_ == GlazingType::GLAZING_ACRYLIC) {
                materialDensity_ = PAPER_PRINT_GLAZING_ACRYLIC_DENSITY;
                perBoxCount_ = GLASS_ACRYLIC_FRAMED_PER_BOX;
            }
            else if(glazeType_ == GlazingType::GLAZING_GLASS) {
                materialDensity_ = PAPER_PRINT_GLAZING_GLASS_DENSITY;
                perBoxCount_ = GLASS_ACRYLIC_FRAMED_PER_BOX;
            }
            else {
                materialDensity_ = 0.0f;  // Default to 0 since we weren't given a density for glazeless paper prints
                perBoxCount_ = GLASS_ACRYLIC_SUNRISE_PER_BOX;
            }
            break;
        case MaterialType::PATIENT_BOARD:
            materialDensity_ = PATIENT_BOARD_DENSITY;
            perBoxCount_ = 0; // for now since unspecified
            break;
        default:
            materialDensity_ = 0.0f;  // Default to 0 if the material is unknown
            perBoxCount_ = 0;
            break;
    }
}

// Getter functions to access private member variables

int Art::getLineNumber() {
    return lineNumber_;
}

std::string Art::getTagNumber() {
    return tagNumber_;
}

MaterialType Art::getMaterial() {
    return material_;
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

bool Art::needsCustomPackaging() {
    // art needs custom packing if dimensions exceed 43.5 x 88 (43.5 x 88 STILL FITS and does NOT need custom packing)
    // --> if any dimension exceeds 88, needs custom
    // --> if both dimensions exceed 43.5, needs custom
    // --> if only ONE dimension exceeds 43.5, do NOT need custom
    if ((outerWidth_ > CUSTOM_PACKING_NEEDED_THRESHOLD_LARGER_DIM || outerHeight_ > CUSTOM_PACKING_NEEDED_THRESHOLD_LARGER_DIM)
        || (outerWidth_ > CUSTOM_PACKING_NEEDED_THRESHOLD_SMALLER_DIM && outerHeight_ > CUSTOM_PACKING_NEEDED_THRESHOLD_SMALLER_DIM)) {
        return true;
    }
    return false;
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
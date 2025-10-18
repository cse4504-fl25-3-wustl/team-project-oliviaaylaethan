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
            break;
        case MaterialType::ACOUSTIC_PANEL_FRAMED:
            materialDensity_ = ACOUSTIC_PANEL_FRAMED_DENSITY;
            break;
        case MaterialType::CANVAS_FRAMED:
            materialDensity_ = CANVAS_FRAMED_DENSITY;
            break;
        case MaterialType::CANVAS_GALLERY:
            materialDensity_ = CANVAS_GALLERY_DENSITY;
            break;
        case MaterialType::MIRROR:
            materialDensity_ = MIRROR_DENSITY;
            break;
        case MaterialType::PAPER_PRINT_FRAMED:
            if(glazeType_ == GlazingType::GLAZING_ACRYLIC) {
                materialDensity_ = PAPER_PRINT_GLAZING_ACRYLIC_DENSITY;
            }
            else if(glazeType_ == GlazingType::GLAZING_GLASS) {
                materialDensity_ = PAPER_PRINT_GLAZING_GLASS_DENSITY;
            }
            else {
                materialDensity_ = 0.0f;  // Default to 0 since we weren't given a density for glazeless paper prints
            }
            break;
        case MaterialType::PATIENT_BOARD:
            materialDensity_ = PATIENT_BOARD_DENSITY;
            break;
        default:
            materialDensity_ = 0.0f;  // Default to 0 if the material is unknown
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

bool Art::needsCustomPackaging() {
    // Anything that exceeds 43.5" in BOTH directions would require custom packaging.
    if (outerWidth_ > CUSTOM_PACKING_NEEDED_THRESHOLD && outerHeight_ > CUSTOM_PACKING_NEEDED_THRESHOLD) {
        return true;
    }
    return false;
}

bool Art::needsCanvasPacking() {
    return material_ == MaterialType::CANVAS_FRAMED || material_ == MaterialType::CANVAS_GALLERY;
}
#include "art.h"

// Default constructor
Art::Art() : lineNumber_(0), quantity_(0), tagNumber_(""), material_(MaterialType::CANVAS_FRAMED),
             materialDensity_(0.0f), outerWidth_(0.0f), outerHeight_(0.0f), glazeType_(GlazingType::GLAZING_NONE),
             frame1Moulding_(""), hardware_("") {}

// Constructor to initialize the Art object
Art::Art(int lineNo, int quantity, std::string tagNo, 
           MaterialType material, float outerWidth, float outerHeight, 
           GlazingType glazeType, std::string frame1Moulding, std::string hardware)
    : lineNumber_(lineNo), 
      quantity_(quantity), 
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
        case MaterialType::ACRYLIC:
            materialDensity_ = ACRYLIC_DENSITY;
            break;
        case MaterialType::CANVAS_FRAMED:
            materialDensity_ = CANVAS_FRAMED_DENSITY;
            break;
        case MaterialType::CANVAS_GALLERY:
            materialDensity_ = CANVAS_GALLERY_DENSITY;
            break;
        case MaterialType::GLASS:
            materialDensity_ = GLASS_DENSITY;
            break;
        case MaterialType::MIRROR:
            materialDensity_ = MIRROR_DENSITY;
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

int Art::getQuantity() {
    return quantity_;
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

std::string Art::getHardware() {
    return hardware_;
}
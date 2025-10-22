#include "response.h"
#include <format>
#include <map>

ArtInfo::ArtInfo(const std::vector<Art>& pieces) : pieces_(pieces) {
    for (auto& art : pieces_) {
        int lineNo = art.getLineNumber();
        quantities_[lineNo]++;
        artTypes_[lineNo] = art;
    }
}

int ArtInfo::getTotalCount() {
    return static_cast<int>(pieces_.size());
}

int ArtInfo::getStandardCount() {
    int totalCount = getTotalCount();
    int count = 0;
    Art art;
    for (int i = 0; i < totalCount; i++) {
        art = pieces_[i];
        if (art.getOuterWidth() <= 33 and art.getOuterHeight() <= 43) {
            count++;
        }
    }
    return count;
}

int ArtInfo::getOversizedCount() {
    int totalCount = getTotalCount();
    int count = 0;
    Art art;
    for (int i = 0; i < totalCount; i++) {
        art = pieces_[i];
        if (art.getOuterWidth() > 33 || art.getOuterHeight() > 43) {
            count++;
        }
    }
    return count;
}

int ArtInfo::getQuantity(int lineNo) {
    return quantities_[lineNo];
}

Art ArtInfo::getArtType(int lineNo) {
    return artTypes_[lineNo];
}

int ArtInfo::getTotalWeight() {
    int totalCount = getTotalCount();
    int weight = 0;
    for (int i = 0; i < totalCount; i++) {
        weight += pieces_[i].getWeight();
    }
    return weight;
}

std::vector<std::string> ArtInfo::getOversizedSummary() {
    std::vector<std::string> summary;
    summary.push_back("\nOversized Items Flagged:");
    for (auto& [lineNo, art] : artTypes_) {
        // Check if oversized
        if (art.getOuterWidth() > 33 and art.getOuterHeight() > 43) {
            summary.push_back(std::format("- {}\"x{}\" (Qty: {}) - Requires large box",
                art.getOuterHeight(),
                art.getOuterWidth(),
                getQuantity(lineNo)
            ));
        }
    }
    return summary;
}

std::vector<std::string> ArtInfo::getTotalWeightSummary() {
    int totalCount = getTotalCount();
    std::map<MaterialType, int> weightByType;
    int oversizedWeight = 0;
    Art art;
    for (int i = 0; i < totalCount; i++) {
        art = pieces_[i];

        // Check if oversized
        if (art.getOuterWidth() > 33 and art.getOuterHeight() > 43) {
            oversizedWeight += art.getWeight();
        }
        else {
            weightByType[art.getMaterial()] += art.getWeight();
        }
    }

    // Now we convert to a string vector
    std::vector<std::string> summary;
    summary.push_back(std::format(
        "\nTotal Artwork Weight: {} lbs", getTotalWeight()));
    if (weightByType[ACOUSTIC_PANEL] > 0) {
        summary.push_back(std::format(
            "- Acoustic panels: {} lbs", weightByType[ACOUSTIC_PANEL]));
    }
    if (weightByType[ACOUSTIC_PANEL_FRAMED] > 0) {
        summary.push_back(std::format(
            "- Acoustic framed panels: {} lbs", weightByType[ACOUSTIC_PANEL_FRAMED]));
    }
    if (weightByType[CANVAS_FRAMED] > 0) {
        summary.push_back(std::format(
            "- Canvas framed prints (acrylic glazing, glass glazing, and no glazing): {} lbs", weightByType[CANVAS_FRAMED])); // TODO split this up by glazing type?
    }
    if (weightByType[CANVAS_GALLERY] > 0) {
        summary.push_back(std::format(
            "- Canvas gallery prints: {} lbs", weightByType[CANVAS_GALLERY]));
    }
    if (weightByType[MIRROR] > 0) {
        summary.push_back(std::format(
            "- Mirror pieces: {} lbs", weightByType[MIRROR]));
    }
    if (weightByType[PATIENT_BOARD] > 0) {
        summary.push_back(std::format(
            "- Patient boards: {} lbs", weightByType[PATIENT_BOARD]));
    }
    if (oversizedWeight > 0) {
        summary.push_back(std::format(
            "- Oversized pieces: {} lbs", oversizedWeight));
    }
    
    return summary;
}


#include "response.h"
#include "../entities/requirements.h"
#include <format>
#include <map>
ArtInfo::ArtInfo(const std::vector<Art>& pieces, std::shared_ptr<Requirements> requirements)
    : pieces_(pieces), requirements_(std::move(requirements)) {
    for (auto& art : pieces_) {
        int lineNo = art.getUniqueID();
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
        if (!art.isOversizedInstallation()) {
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
        if (art.isOversizedInstallation()) {
            count++;
        }
    }
    return count;
}

int ArtInfo::getCustomCount() {
    int totalCount = getTotalCount();
    int count = 0;
    Art art;
    for (int i = 0; i < totalCount; i++) {
        art = pieces_[i];
        float limit = 0.0f;

        if(requirements_->getAcceptsCrates().value_or(false)) {
            limit = CRATE_LIMIT;
        } else {
            limit = LARGE_BOX_LIMIT;
        }
        
        if (art.needsCustomPackaging(limit)) {
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
        // only include the weight of art that is not custom packaged
        // custom art is ignored by our software
        if (!pieces_[i].needsCustomPackaging()) {
            weight += pieces_[i].getWeight();
        }
    }
    return weight;
}

std::vector<std::string> ArtInfo::getOversizedSummary() {
    std::vector<std::string> summary;
    summary.push_back("\nOversized Items Flagged:");
    for (auto& [lineNo, art] : artTypes_) {
        // Check if oversized
        if (art.isOversizedInstallation()) {
            summary.push_back(std::format("- {}\"x{}\" (Qty: {}) - {} lbs each",
                art.getOuterHeight(),
                art.getOuterWidth(),
                getQuantity(lineNo),
                art.getWeight()
            ));
        }
    }
    return summary;
}

std::vector<Art> ArtInfo::getOversizedItems() {
    std::vector<Art> oversized;
    for (auto& art: pieces_) {
        if (art.isOversizedInstallation()) {
            oversized.push_back(art);
        }
    }

    return oversized;
}

std::vector<std::string> ArtInfo::getCustomSummary() {
    std::vector<std::string> summary;
    summary.push_back("\nCustom Packaging Items Flagged:");
    for (auto& [lineNo, art] : artTypes_) {
        // Check if oversized
        if (art.needsCustomPackaging()) {
            summary.push_back(std::format("- {}\"x{}\" (Qty: {}) - {} lbs each",
                art.getOuterHeight(),
                art.getOuterWidth(),
                getQuantity(lineNo),
                art.getWeight()
            ));
        }
    }
    return summary;
}

std::vector<Art> ArtInfo::getCustomItems() {
    std::vector<Art> custom;
    for (auto& art: pieces_) {
        if (art.needsCustomPackaging()) {
            custom.push_back(art);
        }
    }

    return custom;
}

std::vector<std::string> ArtInfo::getTotalWeightSummary() {
    int totalCount = getTotalCount();
    std::map<MaterialType, int> weightByType;
    int oversizedWeight = 0;
    int oversizedCount = 0;
    Art art;
    for (int i = 0; i < totalCount; i++) {
        art = pieces_[i];

        // Check if oversized
        if (art.isOversizedInstallation()) {
            oversizedWeight += art.getWeight();
            oversizedCount++;
        }
        else {
            weightByType[art.getMaterial()] += art.getWeight();
        }
    }

    // Now we convert to a string vector
    std::vector<std::string> summary;
    summary.push_back(std::format(
        "\nTotal Artwork Pieces: {}", totalCount));
    summary.push_back(std::format(
        "- Standard Size Pieces: {}", totalCount - oversizedCount));
    summary.push_back(std::format(
        "- Oversized Pieces: {}", oversizedCount));
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

std::string ArtInfo::getPackedArtSummary(std::vector<Art> containerContents) {
    // print out details of each art piece in the box
    // example: - tagNo: 5 	 38 x 56 	 Acoustic panel - Framed
    std::vector<std::string> summary;
    for (size_t j = 0; j < containerContents.size(); j++) {
        std::string tagNo = containerContents[j].getTagNumber();
        std::string finalMedium = containerContents[j].getRawCSVInputMaterial();
        std::string dimensions = std::format("{} x {}",
            containerContents[j].getOuterWidth(),
            containerContents[j].getOuterHeight());
        summary.push_back(std::format(
            "  - tagNo: {} \t {} \t {}", tagNo, dimensions, finalMedium));
    }

    // Convert the vector of strings into a single string
    std::string result;
    for (const auto& line : summary) {
        result += line + "\n"; // Add each line followed by a newline
    }

    return result;
}
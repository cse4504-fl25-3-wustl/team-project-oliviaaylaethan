#include "response.h"
#include <format>

BoxInfo::BoxInfo(const std::vector<Box>& boxes) : boxes_(boxes) {}

int BoxInfo::getStandardBoxCount() {
    int totalCount = getTotalBoxCount();
    int count = 0;

    for (int i = 0; i < totalCount; i++) {
        if (boxes_[i].getBoxType() == STANDARD_BOX) {
            count++;
        }
    }
    return count;
}

int BoxInfo::getLargeBoxCount() {
    int totalCount = getTotalBoxCount();
    int count = 0;

    for (int i = 0; i < totalCount; i++) {
        if (boxes_[i].getBoxType() == LARGE_BOX) {
            count++;
        }
    }
    return count;
}

int BoxInfo::getTotalBoxCount() {
    return static_cast<int>(boxes_.size());
}

std::vector<std::string> BoxInfo::getBoxRequirementsSummary() {
    return {
        "\nBox Requirements:",
        std::format("- Standard boxes (37\"x11\"x31\"): {} boxes",
            getStandardBoxCount()),
        std::format("- Large boxes (44\"x13\"x48\"): {} boxes",
            getLargeBoxCount()),
        std::format("- Total boxes: {}", getTotalBoxCount())
    };
}

int BoxInfo::getTotalWeight() {
    int totalCount = getTotalBoxCount();
    int weight = 0;

    for (int i = 0; i < totalCount; i++) {
        weight += boxes_[i].getTotalWeight();
    }

    return weight;
}

// for each box, be able to print its contents
std::vector<std::string> BoxInfo::getAllPackedArtSummary() {
    int totalCount = getTotalBoxCount();
    std::vector<std::string> summary;
    summary.push_back("\nArt Packing Summary:");
    Box box;
    for (int i = 0; i < totalCount; i++) {
        box = boxes_[i];
        //ex:  "- Box 1 - Large Box"
        summary.push_back(std::format(
            "\n- Box {} - {}", i+1, to_string(box.getBoxType())));
        // ex:  "   - tagNo: 5 	 38 x 56 	 Acoustic panel - Framed"
        summary.push_back(ArtInfo::getPackedArtSummary(box.getContents()));
    }
    return summary;
}

std::vector<Art> BoxInfo::getAllArt() {
    std::vector<Art> allArt;
    Box box;
    for (int i = 0; i < getTotalBoxCount(); i++) {
        box = boxes_[i];
        std::vector<Art> contents = box.getContents();
        for (size_t j = 0; j < contents.size(); j++) {
            allArt.push_back(contents[j]);
        }
    }
    return allArt;
}

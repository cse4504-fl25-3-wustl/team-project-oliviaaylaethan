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

std::vector<std::string> BoxInfo::getAllPackedArtSummary() {
    int totalCount = getTotalBoxCount();
    std::vector<std::string> summary;
    summary.push_back("\nArt Packing Summary:");
    Box box;
    // for each box, print its contents
    /* example:
        - Box 1 - Large Box
            - tagNo: 5 	 38 x 56 	 Acoustic panel - Framed
            - tagNo: 5 	 38 x 56 	 Acoustic panel - Framed
            - tagNo: 5 	 38 x 56 	 Acoustic panel - Framed
            - tagNo: 5 	 38 x 56 	 Acoustic panel - Framed
    */
    for (int i = 0; i < totalCount; i++) {
        box = boxes_[i];
        summary.push_back(std::format(
            "\n- Box {} - {}", i, to_string(box.getBoxType())));
        std::vector<Art> contents = box.getContents();
        // print out details of each art piece in the box
        for (size_t j = 0; j < contents.size(); j++) {
            std::string tagNo = contents[j].getTagNumber();
            std::string finalMedium = contents[j].getRawCSVInputMaterial();
            std::string dimensions = std::format("{} x {}",
                contents[j].getOuterWidth(),
                contents[j].getOuterHeight());
            summary.push_back(std::format(
                "  - tagNo: {} \t {} \t {}", tagNo, dimensions, finalMedium));
        }
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

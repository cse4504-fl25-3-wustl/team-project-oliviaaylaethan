#include "csvParser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

CsvParser::CsvParser() {}

bool CsvParser::isValidFile(const std::string & filePath) {
    std::ifstream file(filePath);

    // error checking for opening file
    if (!file.is_open()) {
        std::cerr << "Error: Could NOT open file: " << filePath << std::endl;
        return false;
    }
    return true;
}

Request CsvParser::parseFiles(std::string artFilePath, std::string siteFilePath) {
    bool artFileValid = isValidFile(artFilePath);
    bool siteFileValid = isValidFile(siteFilePath);

    if (!artFileValid) {
        return Request();
    }
    
    std::vector<Art> artworks = parseArtCsv(artFilePath);

    Requirements siteRequirements = Requirements();
    if (siteFileValid) {
        siteRequirements = parseRequirementsCsv(siteFilePath);
    }

    // to show parsed data for debugging, won't need in final version
    std::cout << std::endl;
    std::cout << "Total artworks parsed: " << artworks.size() << std::endl;
    std::cout << std::endl;

    return Request(artworks, siteRequirements);
}



// Trim utility
std::string CsvParser::trim(const std::string& str) {
    auto start = str.find_first_not_of(" \t\r\n");
    auto end = str.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

// Map Final Medium to MaterialType
MaterialType CsvParser::mapToMaterial(const std::string& medium) { // FIXME these mappings might be wrong
    std::string m = trim(medium);
    std::transform(m.begin(), m.end(), m.begin(), ::tolower); // Convert to lowercase

    if (m == "paper print - framed") return PAPER_PRINT_FRAMED;
    if (m == "canvas - float frame") return CANVAS_FRAMED;
    if (m == "canvas - gallery") return CANVAS_GALLERY;
    if (m == "print - framed with title plate") return CANVAS_FRAMED;
    if (m == "wall décor") return ACOUSTIC_PANEL;
    if (m == "mirror") return MIRROR;
    if (m == "metal print") return PATIENT_BOARD;
    return ACOUSTIC_PANEL; // default/fallback
}

// Map Glazing string to GlazingType
GlazingType CsvParser::mapToGlazing(const std::string& glaze) {
    std::string g = trim(glaze);
    std::transform(g.begin(), g.end(), g.begin(), ::tolower); // Convert to lowercase

    if (g == "regular glass") return GLAZING_GLASS;
    if (g == "acrylic") return GLAZING_ACRYLIC;
    return GLAZING_NONE;
}

// Map Hardware string to HardwareSpec
HardwareSpec CsvParser::mapToHardware(const std::string& hw) {
    std::string h = trim(hw);
    if (h.find("4 pt") != std::string::npos) return PT_SEC_4;
    return PT_SEC_3;
}

std::vector<std::string> CsvParser::commaSplitter(std::string line) {
    std::stringstream ss(line);
    std::vector<std::string> tokens;
    std::string cell;

    while (std::getline(ss, cell, ',')) {
        tokens.push_back(trim(cell));
    }

    return tokens;
}

// Parse CSV and return vector of Art objects
std::vector<Art> CsvParser::parseArtCsv(const std::string& filename) {
    std::cout << "Parsing Art CSV file: " << filename << std::endl;

    std::vector<Art> artList;
    std::ifstream file(filename);
    std::string line;

    // Skip header
    std::getline(file, line);
    while (std::getline(file, line)) {
        std::replace(line.begin(), line.end(), '\t', ',');
        std::vector<std::string> tokens = commaSplitter(line);

        while (tokens.size() < 9) tokens.push_back("");

        try {
            int lineNo = std::stoi(tokens[0]);
            int qty = std::stoi(tokens[1]);
            std::string tag = tokens[2];
            MaterialType material = mapToMaterial(tokens[3]);
            float width = std::stof(tokens[4]);
            float height = std::stof(tokens[5]);
            GlazingType glaze = mapToGlazing(tokens[6]);
            std::string frame = tokens[7];
            HardwareSpec hw = mapToHardware(tokens[8]);

            for (int i = 0; i < qty; i++) {
                Art art(lineNo, tag, material, width, height, glaze, frame, hw);
                artList.push_back(art);
            }
        } catch (const std::exception& e) {
            std::cerr << "Error parsing line: '" << line << "' -> " << e.what() << std::endl;
            continue;  // skip malformed line
        }
    }
    file.close();

    return artList;
}

Requirements CsvParser::parseRequirementsCsv(const std::string& filename) {
    std::cout << "Parsing Requirements CSV file: " << filename << std::endl;

    Requirements siteRequirements = Requirements();
    std::ifstream file(filename);
    std::string line;

    // Read and skip header
    std::getline(file, line);

    // Get line of requirements
    std::getline(file, line);

    std::replace(line.begin(), line.end(), '\t', ',');
    std::vector<std::string> requirementList = commaSplitter(line);

    try {
        siteRequirements.setJobSiteLocation(requirementList[Requirements::JOB_SITE_LOCATION]);
        siteRequirements.setClientName(requirementList[Requirements::CLIENT_NAME]);
        siteRequirements.setAcceptsPallets(requirementList[Requirements::ACCEPTS_PALLETS]);
        siteRequirements.setAcceptsCrates(requirementList[Requirements::ACCEPTS_CRATES]);
        siteRequirements.setHasLoadingDock(requirementList[Requirements::HAS_LOADING_DOCK]);
        siteRequirements.setNeedsLiftgate(requirementList[Requirements::NEEDS_LIFTGATE]);
        siteRequirements.setNeedsInsideDelivery(requirementList[Requirements::NEEDS_INSIDE_DELIVERY]);
        siteRequirements.setServiceType(requirementList[Requirements::SERVICE_TYPE]);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing requirements: '" << line << "' -> " << e.what() << std::endl;
    }
    return siteRequirements;
}

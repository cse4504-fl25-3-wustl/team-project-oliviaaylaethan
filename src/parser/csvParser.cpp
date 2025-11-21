#include "csvParser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

const std::string CsvParser::ACCEPTS_CRATES_YES = "Y";
const std::string CsvParser::ACCEPTS_CRATES_NO = "N";

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

// --- New Default Requirements Generator ---
Requirements CsvParser::generateDefaultRequirements(const std::string& acceptsCratesValue) {
    Requirements siteRequirements = Requirements();
    
    // Based on your default file: Location,Client,Accepts pallets,Accepts crates,LD Access,LG required,Inside Delivery,Service type
    // N/A,N/A,Y,<Y/N>,N/A,N,N/A,N/A
    
    // Since you don't use enum constants for these fields, we'll use string literal indices 
    // to mimic the structure of the CSV line, ensuring only AcceptsCrates changes.
    std::vector<std::string> defaultRequirementsList = {
        "N/A",  // JOB_SITE_LOCATION
        "N/A",  // CLIENT_NAME
        "Y",    // ACCEPTS_PALLETS (Fixed to Y)
        acceptsCratesValue, // ACCEPTS_CRATES (This is the only dynamic value)
        "N/A",  // HAS_LOADING_DOCK
        "N",    // NEEDS_LIFTGATE
        "N/A",  // NEEDS_INSIDE_DELIVERY
        "N/A"   // SERVICE_TYPE
    };

    try {
        // Assume you have defined the indices in Requirements.h, e.g.,
        // static const int JOB_SITE_LOCATION = 0;
        siteRequirements.setJobSiteLocation(defaultRequirementsList[0]);
        siteRequirements.setClientName(defaultRequirementsList[1]);
        siteRequirements.setAcceptsPallets(defaultRequirementsList[2]);
        siteRequirements.setAcceptsCrates(defaultRequirementsList[3]);
        siteRequirements.setHasLoadingDock(defaultRequirementsList[4]);
        siteRequirements.setNeedsLiftgate(defaultRequirementsList[5]);
        siteRequirements.setNeedsInsideDelivery(defaultRequirementsList[6]);
        siteRequirements.setServiceType(defaultRequirementsList[7]);
    } catch (const std::exception& e) {
        std::cerr << "Error generating default requirements: " << e.what() << std::endl;
    }
    
    return siteRequirements;
}

Request CsvParser::parseFiles(std::string artFilePath, std::string siteFilePath, bool isSubstitute) {
    bool artFileValid = isValidFile(artFilePath);
    bool siteFileValid = isValidFile(siteFilePath);

    if (!artFileValid) {
        std::cerr << "Art file invalid, returning empty request." << std::endl;
        return Request();
    }
    
    std::vector<Art> artworks = parseArtCsv(artFilePath);
    Requirements siteRequirements = Requirements();

    if (isSubstitute) {
        // If it's a substitute ('Y' or 'N' flag)
        std::cout << "Generating requirements from substitute flag: " << siteFilePath << std::endl;
        if (siteFilePath == ACCEPTS_CRATES_YES || siteFilePath == "y") {
            siteRequirements = generateDefaultRequirements(ACCEPTS_CRATES_YES);
        } else { // 'N' or 'n'
            siteRequirements = generateDefaultRequirements(ACCEPTS_CRATES_NO);
        }
    } else {
        // If it's a file path
        bool siteFileValid = isValidFile(siteFilePath);
        if (siteFileValid) {
            siteRequirements = parseRequirementsCsv(siteFilePath);
        } else {
            // File path provided but invalid. Use empty default.
            std::cerr << "Error: Requirements file invalid. Using default (empty) requirements." << std::endl;
        }
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
    if (m == "acoustic panel") return ACOUSTIC_PANEL;
    if (m == "acoustic panel - framed") return ACOUSTIC_PANEL_FRAMED;
    if (m == "mirror") return MIRROR;
    // TODO these don't have their own set material
    if (m == "metal print") return PATIENT_BOARD;
    if (m == "print - framed with title plate") return CANVAS_FRAMED;
    if (m == "wall décor") return ACOUSTIC_PANEL;
    return ACOUSTIC_PANEL; // default/fallback
}

// Map Glazing string to GlazingType
GlazingType CsvParser::mapToGlazing(const std::string& glaze) {
    std::string g = trim(glaze);
    std::transform(g.begin(), g.end(), g.begin(), ::tolower); // Convert to lowercase

    if (g == "regular glass") return GLAZING_GLASS;
    if (g == "glass") return GLAZING_GLASS;
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

    // in case input is badly-formatted (multiple DIFFERENT types of art correspond to 1 line number),
    // use this unique id to mimic properly-formatted line number
    // ex: test case input.csv for 1Large1Standard1Custom puts every type of art as line number 1
    int uniqueArtId = 0;

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
            ++uniqueArtId;

            for (int i = 0; i < qty; i++) {
                Art art(lineNo, tag, material, width, height, glaze, frame, hw, uniqueArtId);
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

#include "csvParser.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include <algorithm>

CsvParser::CsvParser() {}

Request CsvParser::parseFile(std::string filePath) {
    std::cout << "Parsing CSV file: " << filePath << std::endl;


    std::ifstream file(filePath);
    // error checking for opening file
    if (!file.is_open()) {
        std::cerr << "Error: Could NOT open file: " << filePath << std::endl;
    }

    std::vector<Art> artworks = parseCSV(filePath);

    // to show parsed data for debugging, won't need in final version
    for (auto& art : artworks) {
        std::cout << "Line #" << art.getLineNumber()
                  << ", Quantity: " << art.getQuantity()
                  << ", Material: " << art.getMaterial()
                  << ", Glaze: " << art.getGlazeType()
                  << ", Size: " << art.getOuterWidth() << "x" << art.getOuterHeight()
                  << ", Hardware: " << art.getHardware()
                  << std::endl;
    }

    std::cout << "Total artworks parsed: " << artworks.size() << std::endl;

    return Request(artworks);
}

// Trim utility
std::string CsvParser::trim(const std::string& str) {
    auto start = str.find_first_not_of(" \t\r\n");
    auto end = str.find_last_not_of(" \t\r\n");
    return (start == std::string::npos) ? "" : str.substr(start, end - start + 1);
}

// Map Final Medium to MaterialType
MaterialType CsvParser::mapToMaterial(const std::string& medium) {
    std::string m = trim(medium);
    if (m == "Paper Print - Framed") return CANVAS_FRAMED;
    if (m == "Canvas - Float Frame") return CANVAS_GALLERY;
    if (m == "Print - Framed with Title Plate") return CANVAS_FRAMED;
    if (m == "Wall Décor") return ACOUSTIC_PANEL;
    if (m == "Metal Print") return ACRYLIC;
    if (m == "Mirror") return MIRROR;
    return ACOUSTIC_PANEL; // default/fallback
}

// Map Glazing string to GlazingType
GlazingType CsvParser::mapToGlazing(const std::string& glaze) {
    std::string g = trim(glaze);
    if (g == "Regular Glass") return GLAZING_GLASS;
    if (g == "Acrylic") return GLAZING_ACRYLIC;
    return GLAZING_NONE;
}

// Map Hardware string to HardwareSpec
HardwareSpec CsvParser::mapToHardware(const std::string& hw) {
    std::string h = trim(hw);
    if (h.find("4 pt") != std::string::npos) return PT_SEC_4;
    return PT_SEC_3;
}

// Get density based on material
float CsvParser::getMaterialDensity(MaterialType mat) {
    switch (mat) {
        case ACOUSTIC_PANEL: return ACOUSTIC_PANEL_DENSITY;
        case ACOUSTIC_PANEL_FRAMED: return ACOUSTIC_PANEL_FRAMED_DENSITY;
        case ACRYLIC: return ACRYLIC_DENSITY;
        case CANVAS_FRAMED: return CANVAS_FRAMED_DENSITY;
        case CANVAS_GALLERY: return CANVAS_GALLERY_DENSITY;
        case GLASS: return GLASS_DENSITY;
        case MIRROR: return MIRROR_DENSITY;
        case PATIENT_BOARD: return PATIENT_BOARD_DENSITY;
        default: return 0.0f;
    }
}

// Parse CSV and return vector of Art objects
std::vector<Art> CsvParser::parseCSV(const std::string& filename) {
    std::vector<Art> artList;
    std::ifstream file(filename);
    std::string line;

    // Skip header
    std::getline(file, line);
while (std::getline(file, line)) {
    std::replace(line.begin(), line.end(), '\t', ',');
    std::stringstream ss(line);
    std::vector<std::string> tokens;
    std::string cell;

    while (std::getline(ss, cell, ',')) {
        tokens.push_back(trim(cell));
    }
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

        Art art(lineNo, qty, tag, material, width, height, glaze, frame, hw);
        artList.push_back(art);
    } catch (const std::exception& e) {
        std::cerr << "Error parsing line: '" << line << "' -> " << e.what() << std::endl;
        continue;  // skip malformed line
    }
}
    file.close();

    return artList;
}

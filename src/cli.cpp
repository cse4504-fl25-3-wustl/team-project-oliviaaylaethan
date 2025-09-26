#include "cli.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include "item.h"
#include "references/materialsInfo.h"
#include "packingCalculator.h"
#include "weightCalculator.h"

// Function to get the string representation of MaterialType
std::string getMaterialTypeName(MaterialType material) {
    switch (material) {
        case MaterialType::ACOUSTIC_PANEL: return "Acoustic Panel";
        case MaterialType::ACOUSTIC_PANEL_FRAMED: return "Acoustic Panel Framed";
        case MaterialType::ACRYLIC: return "Acrylic";
        case MaterialType::CANVAS_FRAMED: return "Canvas - Framed";
        case MaterialType::CANVAS_GALLERY: return "Canvas - Gallery";
        case MaterialType::GLASS: return "Glass";
        case MaterialType::MIRROR: return "Mirror";
        case MaterialType::PATIENT_BOARD: return "Patient Board";
        default: return "Unknown Material";
    }
}

// Function to get the string representation of GlazingType
std::string getGlazingTypeName(GlazingType glaze) {
    switch (glaze) {
        case GlazingType::GLAZING_ACRYLIC: return "Acrylic";
        case GlazingType::GLAZING_GLASS: return "Glass";
        case GlazingType::GLAZING_NONE: return "No Glazing";
        default: return "Unknown Glazing";
    }
}

// Function to parse a line of input and create an Item
Item createItemFromLine(const std::string& line) {
    std::istringstream stream(line);
    std::string token;

    int lineNumber, quantity;
    std::string tagNumber, materialStr, frame1Moulding, hardware;
    float outerWidth, outerHeight;
    GlazingType glazeType = GlazingType::GLAZING_NONE;  // Default to no glazing

    // Read each value from the line
    std::getline(stream, token, '\t');  // Line Number
    lineNumber = std::stoi(token);

    std::getline(stream, token, '\t');  // Quantity
    quantity = std::stoi(token);

    std::getline(stream, token, '\t');  // Tag Number
    tagNumber = token;

    std::getline(stream, token, '\t');  // Material (Final Medium)
    materialStr = token;

    std::getline(stream, token, '\t');  // Outside Size Width
    outerWidth = std::stof(token);

    std::getline(stream, token, '\t');  // Outside Size Height
    outerHeight = std::stof(token);

    std::getline(stream, token, '\t');  // Glazing Type
    std::string glazeStr = token;
    if (glazeStr == "Regular Glass") {
        glazeType = GlazingType::GLAZING_GLASS;
    } else if (glazeStr == "Acrylic") {
        glazeType = GlazingType::GLAZING_ACRYLIC;
    }

    std::getline(stream, token, '\t');  // Frame 1 Moulding
    frame1Moulding = token;

    std::getline(stream, token, '\t');  // Hardware
    hardware = token;

    // Determine MaterialType based on the material string
    MaterialType material = MaterialType::ACOUSTIC_PANEL;  // Default to some material
    if (materialStr == "Acoustic Panel") {
        material = MaterialType::ACOUSTIC_PANEL;
    } else if (materialStr == "Acoustic Panel Framed") {
        material = MaterialType::ACOUSTIC_PANEL_FRAMED;
    } else if (materialStr == "Acrylic") {
        material = MaterialType::ACRYLIC;
    } else if (materialStr == "Canvas - Framed") {
        material = MaterialType::CANVAS_FRAMED;
    } else if (materialStr == "Canvas - Gallery") {
        material = MaterialType::CANVAS_GALLERY;
    } else if (materialStr == "Glass") {
        material = MaterialType::GLASS;
    } else if (materialStr == "Mirror") {
        material = MaterialType::MIRROR;
    } else if (materialStr == "Patient Board") {
        material = MaterialType::PATIENT_BOARD;
    }

    // Create and return the Item object
    return Item(lineNumber, quantity, tagNumber, material, outerWidth, outerHeight, glazeType, frame1Moulding, hardware);
}

// Function to print created items
void printCreatedItems(std::vector<Item>& items) {
    std::cout << "\nCreated Items:\n";
    for (auto& item : items) {
        std::cout << "Line: " << item.getLineNumber() << "\n";
        std::cout << "Quantity: " << item.getQuantity() << "\n";
        std::cout << "Tag Number: " << item.getTagNumber() << "\n";
        std::cout << "Material Type: " << getMaterialTypeName(item.getMaterial()) << "\n";  // Display material name
        std::cout << "Material Density: " << item.getDensity() << " LB/SQIN\n";
        std::cout << "Dimensions: " << item.getOuterWidth() << " x " << item.getOuterHeight() << "\n";
        std::cout << "Glazing Type: " << getGlazingTypeName(item.getGlazeType()) << "\n";  // Display glazing name
        std::cout << "Frame Moulding: " << item.getFrame1Moulding() << "\n";
        std::cout << "Hardware: " << item.getHardware() << "\n\n";
    }
}

// Function to call and display all the functions of PackingCalculator
void callPackingCalculatorFunctions(std::vector<Item>& items) {
    PackingCalculator packingCalculator(items);

    int standardBoxCount = packingCalculator.getStandardBoxCount();
    int largeBoxCount = packingCalculator.getLargeBoxCount();
    int totalBoxCount = packingCalculator.getTotalBoxCount();
    int totalCrateCount = packingCalculator.getTotalCrateCount();
    int packagingWeight = packingCalculator.getPackagingWeight();
    int standardPalletCount = packingCalculator.getStandardPalletCount();
    int oversizedPalletCount = packingCalculator.getOversizedPalletCount();
    int totalPalletCount = packingCalculator.getTotalPalletCount();
    PalletSpec palletList = packingCalculator.getPalletList();
    std::string lineItemHWSummary = packingCalculator.getLineItemHWSummary();
    int wallHardwareCount = packingCalculator.getWallHardwareCount();
    int drywallAnchorCount = packingCalculator.getDrywallAnchorCount();
    int screwCount = packingCalculator.getScrewCount();
    int tBoltCount = packingCalculator.getTBoltCount();

    // Display results from PackingCalculator
    std::cout << "Packing Calculator Results:\n\n";
    std::cout << "Standard Box Count: " << standardBoxCount << "\n";
    std::cout << "Large Box Count: " << largeBoxCount << "\n";
    std::cout << "Total Box Count: " << totalBoxCount << "\n\n";
    std::cout << "Total Crate Count: " << totalCrateCount << "\n\n";
    std::cout << "Packaging Weight: " << packagingWeight << " lbs\n\n";
    std::cout << "Standard Pallet Count: " << standardPalletCount << "\n";
    std::cout << "Oversized Pallet Count: " << oversizedPalletCount << "\n";
    std::cout << "Total Pallet Count: " << totalPalletCount << "\n\n";
}

// Function to call and display the total weight from WeightCalculator
void callWeightCalculator(std::vector<Item>& items, int standardBoxCount, int largeBoxCount, int totalCrateCount) {
    std::list<Item> itemList(items.begin(), items.end());

    WeightCalculator weightCalculator;
    float totalWeight = weightCalculator.calculateTotalWeight(itemList, standardBoxCount, largeBoxCount, totalCrateCount);

    // Display results from calculator
    std::cout << "Total weight: " << totalWeight << "\n\n";
}

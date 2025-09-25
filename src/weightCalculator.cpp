#include "weightCalculator.h"
#include "references/materialsInfo.h"
#include <string>
#include <list>
#include <cmath>
#include <iostream>

WeightCalculator::WeightCalculator()
: totalPieces_(0),
    standardPieces_(0),
    oversizedPieces_(0),
    oversizedPiecesOutput_(),
    totalArtworkWeight_(0),
    totalPackagingWeight_(0),
    finalShipmentWeight_(0)
{}

void WeightCalculator::addItemWeight(Item item) {
    int weight = std::ceil(item.getOuterHeight() * item.getOuterWidth() * item.getDensity() * item.getQuantity());

    if (item.getOuterHeight() > 43 || item.getOuterWidth() > 33) {
        oversizedPieces_++;
        oversizedPiecesOutput_.push_back("* " + 
            std::to_string(std::ceil(item.getOuterHeight())) + 
            "\" x " +
            std::to_string(std::ceil(item.getOuterWidth())) +
            " (Qty: " +
            std::to_string(item.getQuantity()) +
            ") = " +
            std::to_string(weight) + " lbs");
    }
    else {
        standardPieces_++;
    }
    totalPieces_++;
    totalArtworkWeight_ += weight;
}

void WeightCalculator::setTotalPalletWeight(int standard, int oversized, int crates) {
    totalPackagingWeight_ = std::ceil(standard * 60 + oversized * 75 + crates * 125);
}

void WeightCalculator::calculateFinalShipmentWeight(std::list<Item> items, int standard, int oversized, int crates) {
    // Calculate artwork weight
    for (Item item : items) {
        addItemWeight(item);
    }

    // Calculate packaging weight
    setTotalPalletWeight(standard, oversized, crates);
    
    // Set final shipment weight
    finalShipmentWeight_ = totalArtworkWeight_ + finalShipmentWeight_;
}

std::list<std::string> WeightCalculator::formatWorkOrderSummary() {
    std::list<std::string> outputStrings = std::list<std::string>();

    // Calculate final shipment weight first
    if (totalPieces_ < 0) {
        return outputStrings;
    }

    outputStrings.push_back("Work Order Summary:");
    outputStrings.push_back("- Total Pieces: " +
        std::to_string(totalPieces_));
    outputStrings.push_back("- Standard Size Pieces: " +
        std::to_string(standardPieces_) +
        " (estimated at 43\" x 33\")");
    outputStrings.push_back("- Oversized Pieces: " +
        std::to_string(oversizedPieces_));
    
    // Add list of oversized pieces
    for (std::string str : oversizedPiecesOutput_) {
        outputStrings.push_back(str);
    }

    outputStrings.push_back("\nTotal Artwork Weight: " +
        std::to_string(totalArtworkWeight_) + " lbs");
    outputStrings.push_back("Total Packaging Weight: " +
        std::to_string(totalPackagingWeight_) + " lbs");
    outputStrings.push_back("Final Shipment Weight: " +
    std::to_string(finalShipmentWeight_) + " lbs");

    return outputStrings;
}

void WeightCalculator::printWorkOrderSummary() {
    std::list<std::string> formattedStrings = formatWorkOrderSummary();

    if (formattedStrings.size() == 0) {
        std::cout << "Error printing work order summary. Calculate weight first." << std::endl;

        return;
    }

    for (std::string str : formattedStrings) {
        std::cout << str << std::endl;
    }
}


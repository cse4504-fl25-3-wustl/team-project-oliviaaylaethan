#pragma once

#include <list>
#include <string>
#include "item.h"

class WeightCalculator {
    private:
        int totalPieces_;
        int standardPieces_;
        int oversizedPieces_;
        std::list<std::string> oversizedPiecesOutput_;
        int totalArtworkWeight_;
        int totalPackagingWeight_;
        int finalShipmentWeight_;

        void addItemWeight(Item item);
        void setTotalPalletWeight(int standard, int oversized, int crates);

    public:
        WeightCalculator();
        void calculateFinalShipmentWeight(std::list<Item> items, int standard, int oversized, int crates);
        std::list<std::string> formatWorkOrderSummary();
        void printWorkOrderSummary();
};

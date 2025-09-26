#pragma once

#include <iostream>
#include <vector>
#include <list>
#include <string>
#include "item.h"
#include "references/materialsInfo.h"
#include "packingCalculator.h"
#include "weightCalculator.h"

// get the string representation of these types (instead of printing int from enum)
std::string getMaterialTypeName(MaterialType material);
std::string getGlazingTypeName(GlazingType glaze);

// parse a line of input and create an Item
Item createItemFromLine(const std::string& line);

// print the created items
void printCreatedItems(std::vector<Item>& items);

// call and display all the functions of PackingCalculator
void callPackingCalculatorFunctions(std::vector<Item>& items);

// call and display total weight from WeightCalculator
void callWeightCalculator(std::vector<Item>& items, 
                          int standardBoxCount, int largeBoxCount, int totalCrateCount);

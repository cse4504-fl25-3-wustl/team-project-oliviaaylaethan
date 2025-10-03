#pragma once
#include <string>
#include "../requests/request.h"
#include "../entities/art.h"

class CsvParser{
private:
    std::string trim(const std::string& str);
    MaterialType mapToMaterial(const std::string& medium);
    GlazingType mapToGlazing(const std::string& glaze);
    HardwareSpec mapToHardware(const std::string& hw);
    float getMaterialDensity(MaterialType mat);
    std::vector<Art> parseCSV(const std::string& filePath);
public:
    CsvParser();
    Request parseFile(std::string filePath);
};
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
    std::vector<Art> parseArtCsv(const std::string& filePath);
    Requirements parseRequirementsCsv(const std::string& filePath);
    std::vector<std::string> commaSplitter(std::string line);
public:
    CsvParser();
    bool isValidFile(const std::string& filePath);
    Request parseFiles(std::string artFilePath, std::string siteFilePath);
};
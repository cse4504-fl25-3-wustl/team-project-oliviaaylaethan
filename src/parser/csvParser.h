#pragma once
#include "../constants/clientConstants.h"
#include <string>
#include "../requests/request.h"
#include "../entities/art.h"
#include "../entities/requirements.h"

class CsvParser{
private:
    std::string trim(const std::string& str);
    MaterialType mapToMaterial(const std::string& medium);
    GlazingType mapToGlazing(const std::string& glaze);
    HardwareSpec mapToHardware(const std::string& hw);
    std::vector<Art> parseArtCsv(const std::string& filePath);
    Requirements parseRequirementsCsv(const std::string& filePath);
    std::vector<std::string> commaSplitter(std::string line);
    Requirements generateDefaultRequirements(const std::string acceptsCratesValue);
    
public:
    CsvParser();
    bool isValidFile(const std::string& filePath);
    
    // New method overload to handle Y/N substitute (pass the substitute string as siteFilePath)
    Request parseFiles(std::string artFilePath, std::string siteFilePath, bool isSubstitute);
};
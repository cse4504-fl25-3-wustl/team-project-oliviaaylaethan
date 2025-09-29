#pragma once
#include <string>
#include "../requests/request.h"

class CsvParser{
public:
    CsvParser();
    Request parseFile(std::string filePath);
};
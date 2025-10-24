#pragma once

#include <gtest/gtest.h>
#include <fstream>
#include "../../src/parser/csvParser.h"
#include "../../src/interactors/packingInteractor.h"
#include "../../src/responses/response.h"

class ResponseTest : public ::testing::Test {
public:
    void checkOversizedMatch(std::vector<Art> oversized, std::multiset<std::tuple<float, float>> dimensionSet);
protected:
    CsvParser parser;
    std::string testDataPath;
    std::string requirementsFilePath;
    std::string inputPrefix;
    
    void SetUp() override;

    Response generateResponse(std::string dataInput, std::string requirements);
};
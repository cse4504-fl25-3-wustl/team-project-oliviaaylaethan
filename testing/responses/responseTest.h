#pragma once

#include <fstream>
#include "../../src/parser/csvParser.h"
#include "../../src/interactors/packingInteractor.h"
#include "../../src/responses/response.h"

class ResponseTest {
public:
    static void checkOversizedMatch(std::vector<Art> oversized, std::multiset<std::tuple<float, float>> dimensionSet);
    static Response generateResponse(std::string dataInput, std::string requirements);
};
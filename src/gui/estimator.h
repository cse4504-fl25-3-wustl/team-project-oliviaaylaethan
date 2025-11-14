#pragma once
#include "../interactors/packingInteractor.h"
#include "../parser/csvParser.h"
#include "../responses/response.h"
#include "../responses/responseSummary.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <filesystem>

class Estimator {
    public:
        static std::optional<Response> RunEstimator(int argc, char* argv[]);
};
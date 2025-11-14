#include "interactors/packingInteractor.h"
#include "parser/csvParser.h"
#include "responses/response.h"
#include "responses/responseSummary.h"
#include "gui/estimator.cpp"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <filesystem>

int main(int argc, char* argv[]) {
    std::optional<Response> responseOpt = Estimator::RunEstimator(argc, argv);
    if (!responseOpt.has_value()) {
        std::cout << "Error running estimator." << std::endl;
        return 1;
    }
    return 0;
}
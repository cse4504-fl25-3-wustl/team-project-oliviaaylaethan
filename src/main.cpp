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
    return Estimator::RunEstimator(argc, argv);
}
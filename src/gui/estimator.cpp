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
        static int RunEstimator(int argc, char* argv[]);
};

int Estimator::RunEstimator(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <art_data_file.csv> <requirements_file.csv> optional:<output_file_path>" << std::endl;
        return 1;
    }
    std::string dataInputFile = argv[1];
    std::string requirementsInputFile = argv[2];

    // Configure output file path
    std::string outputFilePath = std::filesystem::current_path().string() + "/output.json"; // default path
    if (argc == 4) {
        outputFilePath = argv[3];
    }

    CsvParser parser;
    if (!parser.isValidFile(dataInputFile) || !parser.isValidFile(requirementsInputFile)) {
        std::cout << "Error: One or both input files are invalid." << std::endl;
        return 1;
    }

    Request request = parser.parseFiles(dataInputFile, requirementsInputFile);
    PackingInteractor packingInteractor;
    Response response = packingInteractor.packAllArt(request);

    // Generate JSON output
    ResponseSummary responseSummary(response);
    nlohmann::json j = responseSummary;
    std::ofstream file(outputFilePath);
    file << j.dump(4);
    file.close();

    return 0;
}
#include "estimator.h"

enum RequirementsFileSubstitute {
    NO_CRATES_ACCEPTED,
    YES_CRATES_ACCEPTED
};


std::optional<Response> Estimator::RunEstimator(int argc, char* argv[]) {
    if (argc < 3 || argc > 4) { // Check minimum 3 and maximum 4 arguments
        std::cerr << "Usage 1: " << argv[0] << " <art_data_file.csv> <requirements_file.csv> optional:<output_file_path>" << std::endl;
        std::cerr << "Usage 2: " << argv[0] << " <art_data_file.csv> <Y or N> optional:<output_file_path>" << std::endl;
        return std::nullopt;
    }
    std::string dataInputFile = argv[1];
    std::string requirementsArg = argv[2]; // This holds the path OR 'Y'/'N'
    
    // Configure output file path
    std::string outputFilePath = std::filesystem::current_path().string() + "/output.json"; // default path
    if (argc == 4) {
        outputFilePath = argv[3];
    }

    CsvParser parser;

    // Check Art Data File validity
    if (!parser.isValidFile(dataInputFile)) {
        std::cerr << "Error: Art data file is invalid: " << dataInputFile << std::endl;
        return std::nullopt;
    }

    // Determine if the second argument is a substitute flag
    bool isSubstitute = false;
    if (requirementsArg == "Y" || requirementsArg == "y" || 
        requirementsArg == "N" || requirementsArg == "n") {
        isSubstitute = true;
    }
    
    // Use the new overloaded parseFiles method
    Request request = parser.parseFiles(dataInputFile, requirementsArg, isSubstitute);
    
    PackingInteractor packingInteractor;
    Response response = packingInteractor.packAllArt(request);

    // Generate JSON output
    ResponseSummary responseSummary(response);
    nlohmann::json j = responseSummary;
    std::ofstream file(outputFilePath);
    file << j.dump(4);
    file.close();

    return response;
}
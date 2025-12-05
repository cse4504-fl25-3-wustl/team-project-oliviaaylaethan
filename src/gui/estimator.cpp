#include "estimator.h"

enum RequirementsFileSubstitute {
    NO_CRATES_ACCEPTED,
    YES_CRATES_ACCEPTED
};


std::optional<Response> Estimator::RunEstimator(int argc, char* argv[]) {

    if (argc < 3) {
        std::cerr << "Error: Missing art data file and/or requirements argument." << std::endl;
        return std::nullopt;
    }

    std::string dataInputFile = argv[1];
    std::string requirementsArg= argv[2];

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

    return response;
}
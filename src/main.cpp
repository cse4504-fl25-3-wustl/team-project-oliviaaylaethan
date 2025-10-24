#include "interactors/packingInteractor.h"
#include "parser/csvParser.h"
#include "responses/response.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cout << "Usage: " << argv[0] << " <art_data_file.csv> <requirements_file.csv>" << std::endl;
        return 1;
    }
    std::string dataInputFile = argv[1];
    std::string requirementsInputFile = argv[2];

    CsvParser parser;
    if (!parser.isValidFile(dataInputFile) || !parser.isValidFile(requirementsInputFile)) {
        std::cout << "Error: One or both input files are invalid." << std::endl;
        return 1;
    }
    
    Request request = parser.parseFiles(dataInputFile, requirementsInputFile);
    PackingInteractor packingInteractor;
    Response response = packingInteractor.packAllArt(request);
    response.printPackingSummary();
    response.printWeightSummary();
    response.printBusinessIntelSummary();

    return 0;
}
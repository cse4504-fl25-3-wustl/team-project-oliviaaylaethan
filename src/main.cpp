#include "gui/estimator.cpp"

int main(int argc, char* argv[]) {
    if (argc < 3 || argc > 4) { // Check minimum 3 arguments or 4 arguments
        std::cerr << "Usage 1: " << argv[0] << " <art_data_file.csv> <requirements_file.csv> optional:<output_file_path>" << std::endl;
        std::cerr << "Usage 2: " << argv[0] << " <art_data_file.csv> <Y or N> optional:<output_file_path>" << std::endl;
        return 1;
    }

    std::optional<Response> responseOpt = Estimator::RunEstimator(argc, argv);
    if (!responseOpt.has_value()) {
        std::cout << "Error running estimator." << std::endl;
        return 1;
    }
    
    // Configure output file path
    std::string outputFilePath = std::filesystem::current_path().string() + "/output.json"; // default path
    if (argc == 4) {
        outputFilePath = argv[3];
    }

    // Generate JSON output
    Response response = responseOpt.value();
    ResponseSummary responseSummary(response);
    nlohmann::json j = responseSummary;
    std::ofstream file(outputFilePath);
    file << j.dump(4);
    file.close();

    return 0;
}
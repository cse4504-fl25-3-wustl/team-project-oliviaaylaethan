#include "responseTest.h"
#include <filesystem>
#include <fstream>
#include <vector>
#include <string>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Feature2Test : public testing::TestWithParam<std::string> {
protected:
    CsvParser parser;
    std::string testDataPath;
    std::string requirementsFilePath;

    void SetUp() override {
        // Set up paths for testing
        testDataPath = std::string(TEST_DATA_PATH) + "/inputsFeature2/"; // Path to test data directory
        requirementsFilePath = std::string(TEST_DATA_PATH) + "/siteRequirements/feature_2_site.csv";
    }

    json loadExpectedOutput(const std::string& csvFilePath) {
        // Locate the expected output file named "expected_output.json" in the same directory as the CSV file
        std::filesystem::path csvPath(csvFilePath);
        std::filesystem::path jsonFilePath = csvPath.parent_path() / "expected_output.json";

        std::ifstream jsonFile(jsonFilePath);
        if (!jsonFile.is_open()) {
            throw std::runtime_error("Failed to open expected output file: " + jsonFilePath.string());
        }

        json expectedOutput;
        jsonFile >> expectedOutput;

        return expectedOutput;
    }
};

TEST_P(Feature2Test, VerifyEndToEndOutput) {
    std::string dataInputFilePath = GetParam(); // Get the current file path
    ASSERT_TRUE(parser.isValidFile(dataInputFilePath)) << "Invalid file: " << dataInputFilePath;

    Response response = ResponseTest::generateResponse(dataInputFilePath, requirementsFilePath);

    // Load expected output from the corresponding .json file
    json expectedOutput = loadExpectedOutput(dataInputFilePath);

    // Perform assertions based on the expected output, ignoring keys not in expectedOutput
    for (const auto& key : {"total_pieces", "standard_box_count", "large_box_count", "crate_count", "custom_piece_count"}) {
        if (!expectedOutput.contains(key)) {
            continue; // Skip keys not present in expectedOutput
        }

        int expectedValue = expectedOutput[key].get<int>();
        int actualValue;

        if (std::string(key).compare("total_pieces") == 0) {
            actualValue = response.getArtInfo().getTotalCount();
        } else if (std::string(key).compare("standard_box_count") == 0) {
            actualValue = response.getBoxInfo().getStandardBoxCount();
        } else if (std::string(key).compare("large_box_count") == 0) {
            actualValue = response.getBoxInfo().getLargeBoxCount();
        } else if (std::string(key).compare("crate_count") == 0) {
            actualValue = response.getCrateInfo().getTotalCrateCount();
        } else if (std::string(key).compare("custom_piece_count") == 0) {
            actualValue = response.getArtInfo().getCustomCount();
        } else {
            FAIL() << "Unexpected key: " << key;
        }

        EXPECT_EQ(expectedValue, actualValue) << "Mismatch for key: " << key;
    }
}

// Helper function to get all test files from the directory
std::vector<std::string> GetTestFiles(const std::string& directory) {
    std::vector<std::string> files;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(directory)) {
        if (entry.is_regular_file() && entry.path().extension() == ".csv") {
            files.push_back(entry.path().string());
        }
    }
    return files;
}

// Instantiate the test suite with file paths
INSTANTIATE_TEST_SUITE_P(
    FileTests,
    Feature2Test,
    testing::ValuesIn(GetTestFiles(std::string(TEST_DATA_PATH) + "/inputsFeature2/"))
);
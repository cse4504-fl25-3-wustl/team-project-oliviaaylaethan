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
        requirementsFilePath = testDataPath + "Site_requirements.csv";
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

        // Ensure all required keys are present and have valid numeric values
        expectedOutput["total_pieces"] = expectedOutput.value("total_pieces", 0);
        expectedOutput["standard_box_count"] = expectedOutput.value("standard_box_count", 0);
        expectedOutput["large_box_count"] = expectedOutput.value("large_box_count", 0);
        expectedOutput["crate_count"] = expectedOutput.value("crate_count", 0);
        expectedOutput["custom_piece_count"] = expectedOutput.value("custom_piece_count", 0);

        return expectedOutput;
    }
};

TEST_P(Feature2Test, VerifyEndToEndOutput) {
    std::string dataInputFilePath = GetParam(); // Get the current file path
    ASSERT_TRUE(parser.isValidFile(dataInputFilePath)) << "Invalid file: " << dataInputFilePath;

    Response response = ResponseTest::generateResponse(dataInputFilePath, requirementsFilePath);

    // Load expected output from the corresponding .json file
    json expectedOutput = loadExpectedOutput(dataInputFilePath);

    // Perform assertions based on the expected output
    EXPECT_EQ(response.getArtInfo().getTotalCount(), expectedOutput["total_pieces"].get<int>())
        << "Mismatch in total pieces.";
    EXPECT_EQ(response.getBoxInfo().getStandardBoxCount(), expectedOutput["standard_box_count"].get<int>())
        << "Mismatch in standard box count.";
    EXPECT_EQ(response.getBoxInfo().getLargeBoxCount(), expectedOutput["large_box_count"].get<int>())
        << "Mismatch in large box count.";
    EXPECT_EQ(response.getCrateInfo().getTotalCrateCount(), expectedOutput["crate_count"].get<int>())
        << "Mismatch in large box count.";
    EXPECT_EQ(response.getArtInfo().getOversizedCount(), expectedOutput["custom_piece_count"].get<int>())
        << "Mismatch in custom piece count.";
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
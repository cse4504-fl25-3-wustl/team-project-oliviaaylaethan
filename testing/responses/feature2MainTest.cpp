#include <gtest/gtest.h>
#include <filesystem>
#include <fstream>
#include <string>
#include <vector>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

class Feature2IntegrationTest : public testing::TestWithParam<std::filesystem::path> {
protected:
    std::string testDataPath;
    std::string requirementsFilePath;
    std::string executablePath;

    void SetUp() override {
        testDataPath = std::string(TEST_DATA_PATH) + "/inputsFeature2/";
        requirementsFilePath = std::string(TEST_DATA_PATH) + "/siteRequirements/feature_2_site.csv";

        // need this for test compatibility between windows and mac
        #ifdef _WIN32
            executablePath = std::filesystem::absolute("main.exe").string();
        #else
            executablePath = std::filesystem::absolute("main").string();
        #endif
    }

    json loadJsonFile(const std::string& path) {
        std::ifstream file(path);
        if (!file.is_open()) {
            throw std::runtime_error("Failed to open JSON file: " + path);
        }
        json j;
        file >> j;
        return j;
    }

    void compareJsonSelective(const nlohmann::json& actual, const nlohmann::json& expected) {
        for (auto it = expected.begin(); it != expected.end(); ++it) {
            const std::string& key = it.key();

            // Skip if expected is null
            if (it.value().is_null()) continue;

            // Fail if actual doesn’t contain the key at all
            ASSERT_TRUE(actual.contains(key)) << "Actual JSON missing key: " << key;

            // Compare values
            std::cout << "[JSON Compare] Checking key: " << key << std::endl;
            EXPECT_EQ(actual[key], it.value()) 
                << "Mismatch for key: \"" << key << "\"\n"
                << "Actual:   " << actual[key].dump(2) << "\n"
                << "Expected: " << it.value().dump(2);

        }
    }


};

TEST_P(Feature2IntegrationTest, CompareProgramOutputToExpected) {
    std::filesystem::path csvPath = GetParam();

    // Expected JSON sits next to input.csv
    std::filesystem::path expectedOutputPath = csvPath.parent_path() / "expected_output.json";

    // Temp directory for outputs, mirroring input structure
    std::filesystem::path relativePath = std::filesystem::relative(csvPath.parent_path(), 
                                      std::string(TEST_DATA_PATH) + "/inputsFeature2");

    std::filesystem::path tempDir = std::filesystem::absolute("temp") / relativePath;
    std::filesystem::create_directories(tempDir);

    std::filesystem::path generatedOutputPath = tempDir / "output.json";

    // Run main
    std::string command = executablePath + " \"" + csvPath.generic_string() + "\" \"" +
                      requirementsFilePath + "\" \"" + generatedOutputPath.generic_string() + "\"";

    //std::cout << "Command: " << command << std::endl;
    int result = std::system(command.c_str());
    ASSERT_EQ(result, 0) << "Executable failed for input: " << csvPath;

    // Load JSONs and compare
    nlohmann::json expected = loadJsonFile(expectedOutputPath.string());
    nlohmann::json actual = loadJsonFile(generatedOutputPath.string());

    compareJsonSelective(actual, expected);
}

std::vector<std::filesystem::path> GetAllCsvFiles(const std::string& rootDir) {
    std::vector<std::filesystem::path> files;
    for (const auto& entry : std::filesystem::recursive_directory_iterator(rootDir)) {
        if (entry.is_regular_file() && entry.path().extension() == ".csv") {
            files.push_back(entry.path());
        }
    }
    return files;
}


// Instantiate with all CSVs
INSTANTIATE_TEST_SUITE_P(
    FileTests,
    Feature2IntegrationTest,
        testing::ValuesIn(GetAllCsvFiles(TEST_DATA_PATH "/inputsFeature2"))
);

#include <gtest/gtest.h>
#include <fstream>
#include "../../src/parser/csvParser.h"
#include "../../src/interactors/packingInteractor.h"
#include "../../src/responses/response.h"

// Test fixture for ShippingContainer tests
class ResponseTest : public ::testing::Test {
protected:
    CsvParser parser;
    std::string testDataPath;
    std::string requirementsFilePath;
    std::string inputPrefix;
    
    void SetUp() override {
        // Create a standard pallet for testing
        testDataPath = std::string(TEST_DATA_PATH) + "/"; // Path to test data directory
        requirementsFilePath = testDataPath + "Site_requirements.csv";
        inputPrefix = "Input";
    }

    Response generateResponse(std::string dataInput, std::string requirements) {
        Request request = parser.parseFiles(dataInput, requirements);

        // check that the file paths are correct (all the other tests will fail if they aren't)
        EXPECT_TRUE(parser.isValidFile(dataInput));
        EXPECT_TRUE(parser.isValidFile(requirements));
    
        PackingInteractor packingInteractor;
        return packingInteractor.packAllArt(request);
    }

    void checkOversizedMatch(std::vector<Art> oversized, std::multiset<std::tuple<float, float>> dimensionSet) {
        for (Art art: oversized) {
            if (art.isOversizedInstallation()) {
                std::tuple<float, float> dimensions = std::make_tuple(art.getOuterWidth(), art.getOuterHeight());
                
                auto extracted = dimensionSet.extract(dimensions);
                EXPECT_FALSE(extracted.empty()) 
                    << "Oversized art with dimensions " << art.getOuterWidth() << " x " 
                    << art.getOuterHeight() << " was not expected.";
                }
        }

        dimensionSet.clear();
    }
};

TEST_F(ResponseTest, Input1_EndToEnd) {
    std::string dataInputFilePath = testDataPath + inputPrefix + "1.csv";
    EXPECT_TRUE(parser.isValidFile(dataInputFilePath));
    Response response = generateResponse(dataInputFilePath, requirementsFilePath);

    EXPECT_EQ(55, response.getArtInfo().getTotalCount());
    EXPECT_EQ(49, response.getArtInfo().getStandardCount());
    EXPECT_EQ(6, response.getArtInfo().getOversizedCount());

    std::vector<Art> oversized = response.getArtInfo().getOversizedItems();

    std::multiset<std::tuple<float, float>> dimensionSet;
    // repeat twice
    for (int i = 0; i < 2; i++) {
        dimensionSet.insert(std::make_tuple(34.0f, 46.0f));
    }

    // only one
    dimensionSet.insert(std::make_tuple(32.0f, 56.0f));

    // repeat thrice
    for (int i = 0; i < 3; i++) {
        dimensionSet.insert(std::make_tuple(32.0f, 48.0f));
    }

    checkOversizedMatch(oversized, dimensionSet);

    // Total Artwork Weight: 784 lbs
    EXPECT_FLOAT_EQ(784, response.getArtInfo().getTotalWeight()) 
        << "Expected total artwork weight of 784 lbs";
    
    // Total Packaging Weight: 150 lbs
    EXPECT_FLOAT_EQ(150, response.getPalletInfo().getTotalTareWeight() + response.getCrateInfo().getTotalTareWeight()) 
        << "Expected total packaging weight of 150 lbs";
    
    // Total Shipment Weight: 934 lbs
    EXPECT_FLOAT_EQ(934, response.getArtInfo().getTotalWeight() + response.getPalletInfo().getTotalTareWeight() + response.getCrateInfo().getTotalTareWeight()) 
        << "Expected total shipment weight of 934 lbs (784 + 150)";
}

TEST_F(ResponseTest, Input2_EndToEnd) {
    std::string dataInputFilePath = testDataPath + inputPrefix + "2.csv";
    EXPECT_TRUE(parser.isValidFile(dataInputFilePath));
    Response response = generateResponse(dataInputFilePath, requirementsFilePath);

    EXPECT_EQ(70, response.getArtInfo().getTotalCount());
    EXPECT_EQ(0, response.getArtInfo().getStandardCount());
    EXPECT_EQ(70, response.getArtInfo().getOversizedCount());

    std::vector<Art> oversized = response.getArtInfo().getOversizedItems();

    std::multiset<std::tuple<float, float>> dimensionSet;
    
    // repeat 70 times
    for (int i = 0; i < 70; i++) {
        dimensionSet.insert(std::make_tuple(36.0f, 44.0f));
    }

    checkOversizedMatch(oversized, dimensionSet);

    EXPECT_FLOAT_EQ(1120, response.getArtInfo().getTotalWeight()) 
        << "Expected total artwork weight of 1120 lbs";
    
    EXPECT_FLOAT_EQ(180, response.getPalletInfo().getTotalTareWeight() + response.getCrateInfo().getTotalTareWeight()) 
        << "Expected total packaging weight of 180 lbs";
    
    EXPECT_FLOAT_EQ(1300, response.getArtInfo().getTotalWeight() + response.getPalletInfo().getTotalTareWeight() + response.getCrateInfo().getTotalTareWeight()) 
        << "Expected total shipment weight of 1300 lbs (1120 + 180)";
}

TEST_F(ResponseTest, Input3_EndToEnd) {
    std::string dataInputFilePath = testDataPath + inputPrefix + "3.csv";
    EXPECT_TRUE(parser.isValidFile(dataInputFilePath));
    Response response = generateResponse(dataInputFilePath, requirementsFilePath);

    EXPECT_EQ(13, response.getArtInfo().getTotalCount());
    EXPECT_EQ(11, response.getArtInfo().getStandardCount());
    EXPECT_EQ(2, response.getArtInfo().getOversizedCount());

    std::vector<Art> oversized = response.getArtInfo().getOversizedItems();

    std::multiset<std::tuple<float, float>> dimensionSet;
    dimensionSet.insert(std::make_tuple(31.0f, 55.0f));
    dimensionSet.insert(std::make_tuple(34.0f, 47.0f));

    checkOversizedMatch(oversized, dimensionSet);

    EXPECT_FLOAT_EQ(187, response.getArtInfo().getTotalWeight()) 
        << "Expected total artwork weight of 187 lbs";
    
    EXPECT_FLOAT_EQ(60, response.getPalletInfo().getTotalTareWeight() + response.getCrateInfo().getTotalTareWeight()) 
        << "Expected total packaging weight of 60 lbs";
    
    EXPECT_FLOAT_EQ(247, response.getArtInfo().getTotalWeight() + response.getPalletInfo().getTotalTareWeight() + response.getCrateInfo().getTotalTareWeight()) 
        << "Expected total shipment weight of 247 lbs (187 + 60)";
}

TEST_F(ResponseTest, Input4_EndToEnd) {
    std::string dataInputFilePath = testDataPath + inputPrefix + "4.csv";
    EXPECT_TRUE(parser.isValidFile(dataInputFilePath));
    Response response = generateResponse(dataInputFilePath, requirementsFilePath);

    EXPECT_EQ(18, response.getArtInfo().getTotalCount());
    EXPECT_EQ(18, response.getArtInfo().getStandardCount());
    EXPECT_EQ(0, response.getArtInfo().getOversizedCount());

    EXPECT_FLOAT_EQ(234, response.getArtInfo().getTotalWeight()) 
        << "Expected total artwork weight of 234 lbs";
    
    EXPECT_FLOAT_EQ(60, response.getPalletInfo().getTotalTareWeight() + response.getCrateInfo().getTotalTareWeight()) 
        << "Expected total packaging weight of 60 lbs";
    
    EXPECT_FLOAT_EQ(294, response.getArtInfo().getTotalWeight() + response.getPalletInfo().getTotalTareWeight() + response.getCrateInfo().getTotalTareWeight()) 
        << "Expected total shipment weight of 294 lbs (234 + 60)";
}
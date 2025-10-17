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
        testDataPath = "../testing/testdata/";
        requirementsFilePath = testDataPath + "Site_requirements.csv";
        inputPrefix = "Input";
    }

    Response generateResponse(std::string dataInput, std::string requirements) {
        Request request = parser.parseFiles(dataInput, requirements);
        PackingInteractor packingInteractor;
        return packingInteractor.packAllArt(request);
    }
};

TEST_F(ResponseTest, Input1_EndToEnd) {
    std::string dataInputFilePath = testDataPath + inputPrefix + "1.csv";
    Response response = generateResponse(dataInputFilePath, requirementsFilePath);

    EXPECT_EQ(55, response.getArtInfo().getTotalCount());
    EXPECT_EQ(49, response.getArtInfo().getStandardCount());
    EXPECT_EQ(6, response.getArtInfo().getOversizedCount());

    // TODO: test oversized pieces summary!
    // this might need to get a list of what's oversized instead
    // response.getArtInfo().getOversizedSummary();

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
    Response response = generateResponse(dataInputFilePath, requirementsFilePath);

    EXPECT_EQ(70, response.getArtInfo().getTotalCount());
    EXPECT_EQ(70, response.getArtInfo().getStandardCount());
    EXPECT_EQ(0, response.getArtInfo().getOversizedCount());

    EXPECT_FLOAT_EQ(1120, response.getArtInfo().getTotalWeight()) 
        << "Expected total artwork weight of 1120 lbs";
    
    EXPECT_FLOAT_EQ(180, response.getPalletInfo().getTotalTareWeight() + response.getCrateInfo().getTotalTareWeight()) 
        << "Expected total packaging weight of 180 lbs";
    
    EXPECT_FLOAT_EQ(1300, response.getArtInfo().getTotalWeight() + response.getPalletInfo().getTotalTareWeight() + response.getCrateInfo().getTotalTareWeight()) 
        << "Expected total shipment weight of 1300 lbs (1120 + 180)";
}

TEST_F(ResponseTest, Input3_EndToEnd) {
    std::string dataInputFilePath = testDataPath + inputPrefix + "3.csv";
    Response response = generateResponse(dataInputFilePath, requirementsFilePath);

    EXPECT_EQ(13, response.getArtInfo().getTotalCount());
    EXPECT_EQ(11, response.getArtInfo().getStandardCount());
    EXPECT_EQ(2, response.getArtInfo().getOversizedCount());

    // TODO: test oversized pieces summary!
    // this might need to get a list of what's oversized instead
    // response.getArtInfo().getOversizedSummary();

    EXPECT_FLOAT_EQ(187, response.getArtInfo().getTotalWeight()) 
        << "Expected total artwork weight of 187 lbs";
    
    EXPECT_FLOAT_EQ(60, response.getPalletInfo().getTotalTareWeight() + response.getCrateInfo().getTotalTareWeight()) 
        << "Expected total packaging weight of 60 lbs";
    
    EXPECT_FLOAT_EQ(1300, response.getArtInfo().getTotalWeight() + response.getPalletInfo().getTotalTareWeight() + response.getCrateInfo().getTotalTareWeight()) 
        << "Expected total shipment weight of 247 lbs (187 + 60)";
}

TEST_F(ResponseTest, Input4_EndToEnd) {
    std::string dataInputFilePath = testDataPath + inputPrefix + "3.csv";
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
#include <gtest/gtest.h>
#include <fstream>
#include "../../src/parser/csvParser.h"

// Test fixture for CsvParser tests
class CsvParserTest : public ::testing::Test {
protected:
    CsvParser parser;
    std::string testDataPath;
    
    void SetUp() override {
        // Path to test data directory
        testDataPath = "../testing/testdata/";
    }
};

// ==================== parseFiles Tests ====================

TEST_F(CsvParserTest, ParseFiles_ValidFiles) {
    // Arrange
    std::string artFile = testDataPath + "test_art.csv";
    std::string reqFile = testDataPath + "test_requirements.csv";
    
    // Act
    Request request = parser.parseFiles(artFile, reqFile);
    
    // Assert - Check that artworks were parsed
    std::vector<Art> artPieces = request.getArtPieces();
    EXPECT_EQ(artPieces.size(), 4); // 4 artworks in test file
    
    // Verify first artwork
    EXPECT_EQ(artPieces[0].getLineNumber(), 1);
    EXPECT_FLOAT_EQ(artPieces[0].getOuterWidth(), 24.0f);
    EXPECT_FLOAT_EQ(artPieces[0].getOuterHeight(), 30.0f);
    
    // Verify requirements were parsed
    Requirements requirements = request.getRequirements();
    EXPECT_EQ(requirements.getJobSiteLocation(), "New York Office");
    EXPECT_EQ(requirements.getClientName(), "Acme Corp");
}

TEST_F(CsvParserTest, ParseFiles_InvalidArtFile) {
    // Arrange
    std::string invalidArtFile = testDataPath + "nonexistent_art.csv";
    std::string validReqFile = testDataPath + "test_requirements.csv";
    
    // Act
    Request request = parser.parseFiles(invalidArtFile, validReqFile);
    
    // Assert - Should return empty Request
    std::vector<Art> artPieces = request.getArtPieces();
    EXPECT_EQ(artPieces.size(), 0);
}

TEST_F(CsvParserTest, ParseFiles_InvalidRequirementsFile) {
    // Arrange
    std::string validArtFile = testDataPath + "test_art.csv";
    std::string invalidReqFile = testDataPath + "nonexistent_requirements.csv";
    
    // Act
    Request request = parser.parseFiles(validArtFile, invalidReqFile);
    
    // Assert - Should still parse art file
    std::vector<Art> artPieces = request.getArtPieces();
    EXPECT_EQ(artPieces.size(), 4);
}

TEST_F(CsvParserTest, ParseFiles_EmptyArtFile) {
    // Arrange
    std::string emptyArtFile = testDataPath + "empty_art.csv";
    std::string validReqFile = testDataPath + "test_requirements.csv";
    
    // Act
    Request request = parser.parseFiles(emptyArtFile, validReqFile);
    
    // Assert - Should have no artworks
    std::vector<Art> artPieces = request.getArtPieces();
    EXPECT_EQ(artPieces.size(), 0);
    
    // But requirements should still be parsed
    Requirements requirements = request.getRequirements();
    EXPECT_EQ(requirements.getJobSiteLocation(), "New York Office");
}

TEST_F(CsvParserTest, ParseFiles_BothInvalidFiles) {
    // Arrange
    std::string invalidArtFile = testDataPath + "nonexistent_art.csv";
    std::string invalidReqFile = testDataPath + "nonexistent_requirements.csv";
    
    // Act
    Request request = parser.parseFiles(invalidArtFile, invalidReqFile);
    
    // Assert - Should return empty Request
    std::vector<Art> artPieces = request.getArtPieces();
    EXPECT_EQ(artPieces.size(), 0);
}

TEST_F(CsvParserTest, ParseFiles_VerifyArtworkDetails) {
    // Arrange
    std::string artFile = testDataPath + "test_art.csv";
    std::string reqFile = testDataPath + "test_requirements.csv";
    
    // Act
    Request request = parser.parseFiles(artFile, reqFile);
    std::vector<Art> artPieces = request.getArtPieces();
    
    // Assert
    ASSERT_EQ(artPieces.size(), 4);
    
    // First artwork - Paper Print Framed
    EXPECT_EQ(artPieces[0].getLineNumber(), 1);
    EXPECT_FLOAT_EQ(artPieces[0].getOuterWidth(), 24.0f);
    EXPECT_FLOAT_EQ(artPieces[0].getOuterHeight(), 30.0f);
    EXPECT_EQ(artPieces[0].getMaterial(), CANVAS_FRAMED); // Paper Print maps to CANVAS_FRAMED
    EXPECT_EQ(artPieces[0].getGlazeType(), GLAZING_GLASS);
    
    // Second artwork - Canvas Float Frame
    EXPECT_EQ(artPieces[1].getLineNumber(), 2);
    EXPECT_FLOAT_EQ(artPieces[1].getOuterWidth(), 36.0f);
    EXPECT_FLOAT_EQ(artPieces[1].getOuterHeight(), 48.0f);
    EXPECT_EQ(artPieces[1].getMaterial(), CANVAS_GALLERY); // Canvas Float Frame maps to CANVAS_GALLERY

    // Second artwork - Canvas Float Frame
    EXPECT_EQ(artPieces[2].getLineNumber(), 2);
    EXPECT_FLOAT_EQ(artPieces[2].getOuterWidth(), 36.0f);
    EXPECT_FLOAT_EQ(artPieces[2].getOuterHeight(), 48.0f);
    EXPECT_EQ(artPieces[2].getMaterial(), CANVAS_GALLERY); // Canvas Float Frame maps to CANVAS_GALLERY
    
    // Third artwork - Metal Print
    EXPECT_EQ(artPieces[3].getLineNumber(), 3);
    EXPECT_FLOAT_EQ(artPieces[3].getOuterWidth(), 20.0f);
    EXPECT_FLOAT_EQ(artPieces[3].getOuterHeight(), 20.0f);
    // TODO: Change this to whatever metal print is supposed to be eventually, could be PATIENT_BOARD?
    // EXPECT_EQ(artPieces[3].getMaterial(), ACRYLIC);
    // EXPECT_EQ(artPieces[3].getGlazeType(), GLAZING_ACRYLIC);
}

TEST_F(CsvParserTest, ParseFiles_VerifyRequirementsDetails) {
    // Arrange
    std::string artFile = testDataPath + "test_art.csv";
    std::string reqFile = testDataPath + "test_requirements.csv";
    
    // Act
    Request request = parser.parseFiles(artFile, reqFile);
    Requirements requirements = request.getRequirements();
    
    // Assert - Verify all requirements fields
    EXPECT_EQ(requirements.getJobSiteLocation(), "New York Office");
    EXPECT_EQ(requirements.getClientName(), "Acme Corp");
    EXPECT_EQ(requirements.getServiceType(), "Standard");
    
    // Verify optional boolean fields
    ASSERT_TRUE(requirements.getAcceptsPallets().has_value());
    EXPECT_TRUE(requirements.getAcceptsPallets().value());
    
    ASSERT_TRUE(requirements.getAcceptsCrates().has_value());
    EXPECT_FALSE(requirements.getAcceptsCrates().value());
    
    ASSERT_TRUE(requirements.getHasLoadingDock().has_value());
    EXPECT_TRUE(requirements.getHasLoadingDock().value());
    
    ASSERT_TRUE(requirements.getNeedsLiftgate().has_value());
    EXPECT_FALSE(requirements.getNeedsLiftgate().value());
    
    ASSERT_TRUE(requirements.getNeedsInsideDelivery().has_value());
    EXPECT_TRUE(requirements.getNeedsInsideDelivery().value());
}

// ==================== isValidFile Tests ====================

TEST_F(CsvParserTest, IsValidFile_ValidFile) {
    // Arrange
    std::string validFile = testDataPath + "test_art.csv";
    
    // Act
    bool isValid = parser.isValidFile(validFile);
    
    // Assert
    EXPECT_TRUE(isValid);
}

TEST_F(CsvParserTest, IsValidFile_InvalidFile) {
    // Arrange
    std::string invalidFile = testDataPath + "nonexistent.csv";
    
    // Act
    bool isValid = parser.isValidFile(invalidFile);
    
    // Assert
    EXPECT_FALSE(isValid);
}


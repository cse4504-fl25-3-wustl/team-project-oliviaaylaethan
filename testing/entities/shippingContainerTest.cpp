#include <gtest/gtest.h>

#include "../../src/entities/shippingContainer.h"

// calculateContainerHeight, addBox

// Test fixture for ShippingContainer tests
class ShippingContainerTest : public ::testing::Test {
protected:
    ShippingContainer container;
    
    void SetUp() override {
        // Create a standard pallet for testing
        container = ShippingContainer::makeStandardPallet();
    }
};

// ==================== addBox Tests ====================

TEST_F(ShippingContainerTest, AddBox_EmptyContainer) {
    // Arrange
    Box box = Box::makeStandardBox();
    
    // Act
    container.addBox(box);
    
    // Assert
    EXPECT_EQ(container.getContents().size(), 1);
}

TEST_F(ShippingContainerTest, AddBox_MultipleBoxes) {
    // Arrange
    Box box1 = Box::makeStandardBox();
    Box box2 = Box::makeLargeBox();
    Box box3 = Box::makeUPSSmallBox();
    
    // Act
    container.addBox(box1);
    container.addBox(box2);
    container.addBox(box3);
    
    // Assert
    EXPECT_EQ(container.getContents().size(), 3);
}

TEST_F(ShippingContainerTest, AddBox_PreservesBoxType) {
    // Arrange
    Box standardBox = Box::makeStandardBox();
    Box largeBox = Box::makeLargeBox();
    
    // Act
    container.addBox(standardBox);
    container.addBox(largeBox);
    
    // Assert
    std::vector<Box> contents = container.getContents();
    EXPECT_EQ(contents[0].getBoxType(), STANDARD_BOX);
    EXPECT_EQ(contents[1].getBoxType(), LARGE_BOX);
}

TEST_F(ShippingContainerTest, AddBox_PreservesDimensions) {
    // Arrange
    Box box = Box::makeStandardBox();
    Dimensions originalDimensions = box.getDimensions();
    
    // Act
    container.addBox(box);
    
    // Assert
    std::vector<Box> contents = container.getContents();
    Dimensions storedDimensions = contents[0].getDimensions();
    EXPECT_FLOAT_EQ(storedDimensions.l, originalDimensions.l);
    EXPECT_FLOAT_EQ(storedDimensions.w, originalDimensions.w);
    EXPECT_FLOAT_EQ(storedDimensions.h, originalDimensions.h);
}

TEST_F(ShippingContainerTest, AddBox_ToCrate) {
    // Arrange
    ShippingContainer crate = ShippingContainer::makeStandardCrate();
    Box box = Box::makeStandardBox();
    
    // Act
    crate.addBox(box);
    
    // Assert
    EXPECT_EQ(crate.getContents().size(), 1);
}

TEST_F(ShippingContainerTest, AddBox_ToOversizePallet) {
    // Arrange
    ShippingContainer oversizePallet = ShippingContainer::makeOversizePallet();
    Box box = Box::makeLargeBox();
    
    // Act
    oversizePallet.addBox(box);
    
    // Assert
    EXPECT_EQ(oversizePallet.getContents().size(), 1);
}

// ==================== calculateContainerHeight Tests ====================

TEST_F(ShippingContainerTest, CalculateHeight_EmptyContainer) {
    // Act
    float height = container.calculateContainerHeight();
    
    // Assert - Empty container should return 8.0 (base height buffer)
    EXPECT_FLOAT_EQ(height, 8.0f);
}

TEST_F(ShippingContainerTest, CalculateHeight_SingleStandardBox) {
    // Arrange
    Box box = Box::makeStandardBox();
    container.addBox(box);
    
    // Act
    float height = container.calculateContainerHeight();
    
    // Assert - Standard box height (31) + 8 inch buffer
    EXPECT_FLOAT_EQ(height, 31.0f + 8.0f);
}

TEST_F(ShippingContainerTest, CalculateHeight_SingleLargeBox) {
    // Arrange
    Box box = Box::makeLargeBox();
    container.addBox(box);
    
    // Act
    float height = container.calculateContainerHeight();
    
    // Assert - Large box height (48) + 8 inch buffer
    EXPECT_FLOAT_EQ(height, 48.0f + 8.0f);
}

TEST_F(ShippingContainerTest, CalculateHeight_MultipleBoxesSameHeight) {
    // Arrange
    Box box1 = Box::makeStandardBox();
    Box box2 = Box::makeStandardBox();
    Box box3 = Box::makeStandardBox();
    container.addBox(box1);
    container.addBox(box2);
    container.addBox(box3);
    
    // Act
    float height = container.calculateContainerHeight();
    
    // Assert - All boxes are 31 inches tall, so max is 31 + 8
    EXPECT_FLOAT_EQ(height, 31.0f + 8.0f);
}

TEST_F(ShippingContainerTest, CalculateHeight_MultipleBoxesDifferentHeights) {
    // Arrange
    Box standardBox = Box::makeStandardBox();  // height = 31
    Box largeBox = Box::makeLargeBox();        // height = 48
    Box upsSmallBox = Box::makeUPSSmallBox();  // height = 36
    
    container.addBox(standardBox);
    container.addBox(largeBox);
    container.addBox(upsSmallBox);
    
    // Act
    float height = container.calculateContainerHeight();
    
    // Assert - Tallest box is 48 inches, so height should be 48 + 8
    EXPECT_FLOAT_EQ(height, 48.0f + 8.0f);
}

TEST_F(ShippingContainerTest, CalculateHeight_FindsMaximumHeight) {
    // Arrange
    Box box1 = Box::makeUPSSmallBox();   // height = 36
    Box box2 = Box::makeStandardBox();   // height = 31
    Box box3 = Box::makeLargeBox();      // height = 48 (tallest)
    Box box4 = Box::makeUPSLargeBox();   // height = 35
    
    container.addBox(box1);
    container.addBox(box2);
    container.addBox(box3);
    container.addBox(box4);
    
    // Act
    float height = container.calculateContainerHeight();
    
    // Assert - Should return the tallest box (48) + 8
    EXPECT_FLOAT_EQ(height, 48.0f + 8.0f);
}

TEST_F(ShippingContainerTest, CalculateHeight_InCrate) {
    // Arrange
    ShippingContainer crate = ShippingContainer::makeStandardCrate();
    Box box = Box::makeLargeBox();
    crate.addBox(box);
    
    // Act
    float height = crate.calculateContainerHeight();
    
    // Assert
    EXPECT_FLOAT_EQ(height, 48.0f + 8.0f);
}

TEST_F(ShippingContainerTest, CalculateHeight_InOversizePallet) {
    // Arrange
    ShippingContainer oversizePallet = ShippingContainer::makeOversizePallet();
    Box box1 = Box::makeStandardBox();
    Box box2 = Box::makeLargeBox();
    oversizePallet.addBox(box1);
    oversizePallet.addBox(box2);
    
    // Act
    float height = oversizePallet.calculateContainerHeight();
    
    // Assert - Should use tallest box
    EXPECT_FLOAT_EQ(height, 48.0f + 8.0f);
}

// ==================== Integration Tests ====================

TEST_F(ShippingContainerTest, AddBoxThenCalculateHeight_Integration) {
    // Arrange - Start with empty container
    EXPECT_EQ(container.getContents().size(), 0);
    EXPECT_FLOAT_EQ(container.calculateContainerHeight(), 8.0f);
    
    // Act & Assert - Add boxes and verify height updates
    Box box1 = Box::makeStandardBox();
    container.addBox(box1);
    EXPECT_FLOAT_EQ(container.calculateContainerHeight(), 31.0f + 8.0f);
    
    Box box2 = Box::makeLargeBox();
    container.addBox(box2);
    EXPECT_FLOAT_EQ(container.calculateContainerHeight(), 48.0f + 8.0f);
    
    Box box3 = Box::makeUPSSmallBox();
    container.addBox(box3);
    EXPECT_FLOAT_EQ(container.calculateContainerHeight(), 48.0f + 8.0f);
    
    // Verify all boxes are present
    EXPECT_EQ(container.getContents().size(), 3);
}
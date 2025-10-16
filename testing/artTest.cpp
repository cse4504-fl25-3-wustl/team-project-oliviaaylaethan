#include <gtest/gtest.h>
#include "..\src\entities\art.h"

// // Demonstrate some basic assertions.
// TEST(HelloTest, BasicAssertions) {
//   // Expect two strings not to be equal.
//   EXPECT_STRNE("hello", "world");
//   // Expect equality.
//   EXPECT_EQ(7 * 6, 42);
// }



// * Art
//       * getWeight
//       * needsCustomShipping


// TODO delete this one
TEST(ArtTest, ReturnsCorrectLineNumber) {
    // Arrange
    int expectedLineNumber = 42;
    Art art(expectedLineNumber, "0", 
        ACOUSTIC_PANEL, 0, 0, 
        GLAZING_NONE, "", NONE);

    // Act
    int actualLineNumber = art.getLineNumber();

    // Assert
    EXPECT_EQ(actualLineNumber, expectedLineNumber);
}

TEST(ArtTest, getWeight) {
    // Arrange
    Art art1(1, "0", ACOUSTIC_PANEL, 24, 36, GLAZING_NONE, "", NONE); // 3.28
    Art art2(2, "0", ACOUSTIC_PANEL_FRAMED, 24, 36, GLAZING_NONE, "", NONE); // 3.20
    Art art3(3, "0", CANVAS_FRAMED, 24, 36, GLAZING_ACRYLIC, "", NONE); // 8.12
    Art art4(4, "0", CANVAS_FRAMED, 24, 36, GLAZING_GLASS, "", NONE); // 8.47
    Art art5(5, "0", CANVAS_FRAMED, 24, 36, GLAZING_NONE, "", NONE); // 7.34
    Art art6(6, "0", CANVAS_GALLERY, 24, 36, GLAZING_NONE, "", NONE); // 5.27
    Art art7(7, "0", MIRROR, 24, 36, GLAZING_NONE, "", NONE); // 16.50
    Art art8(8, "0", PATIENT_BOARD, 24, 36, GLAZING_NONE, "", NONE); // 29.98

    // Act
    int weight1 = art1.getWeight();
    int weight2 = art2.getWeight();
    int weight3 = art3.getWeight();
    int weight4 = art4.getWeight();
    int weight5 = art5.getWeight();
    int weight6 = art6.getWeight();
    int weight7 = art7.getWeight();
    int weight8 = art8.getWeight();

    // Assert
    EXPECT_EQ(weight1, 4);
    EXPECT_EQ(weight2, 4);
    EXPECT_EQ(weight3, 9);
    EXPECT_EQ(weight4, 9);
    EXPECT_EQ(weight5, 8);
    EXPECT_EQ(weight6, 6);
    EXPECT_EQ(weight7, 17);
    EXPECT_EQ(weight8, 30);
}
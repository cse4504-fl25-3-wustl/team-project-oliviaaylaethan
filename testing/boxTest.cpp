#include <gtest/gtest.h>
#include "../src/entities/box.h"

//    * Box
//       * fitsArt
//       * addArt
//       * getTotalWeight

// Rule: As long as at least ONE dimension of an art piece is 36" or less, it will fit in a standard size box. Boxes can be telescoped to a max height of 84"
TEST (BoxTest, fitsArt) {
    // Arrange
    Box box1 = Box::makeStandardBox();
    Box box2= Box::makeLargeBox();

    Art art1(1, "0", ACOUSTIC_PANEL, 24, 32, GLAZING_NONE, "", NONE); // fits in standard box
    Art art2(2, "0", CANVAS_FRAMED, 40, 36, GLAZING_NONE, "", NONE); // fits in standard box
    Art art3(3, "0", CANVAS_FRAMED, 36, 36, GLAZING_NONE, "", NONE); // fits in standard box
    Art art4(4, "0", ACOUSTIC_PANEL_FRAMED, 37, 38, GLAZING_NONE, "", NONE); // fits in large box
    Art art5(5, "0", CANVAS_FRAMED, 50, 50, GLAZING_ACRYLIC, "", NONE); // too big for any box

    // Act & Assert
    EXPECT_TRUE(box1.fitsArt(art1));
    EXPECT_TRUE(box1.fitsArt(art2));
    EXPECT_TRUE(box1.fitsArt(art3));
    EXPECT_FALSE(box1.fitsArt(art4));
    EXPECT_FALSE(box1.fitsArt(art5));

    EXPECT_TRUE(box2.fitsArt(art1));
    EXPECT_TRUE(box2.fitsArt(art2));
    EXPECT_TRUE(box2.fitsArt(art3));
    EXPECT_TRUE(box2.fitsArt(art4));
    EXPECT_FALSE(box2.fitsArt(art5));
}

TEST (BoxTest, addArt) {
    // Arrange
    Box box = Box::makeStandardBox();

    Art art1(1, "0", ACOUSTIC_PANEL, 24, 36, GLAZING_NONE, "", NONE); // fits in standard box
    Art art2(2, "0", ACOUSTIC_PANEL_FRAMED, 34, 36, GLAZING_NONE, "", NONE); // fits in oversized box

    // Act & Assert
    EXPECT_TRUE(box.addArt(art1)); // should succeed
    EXPECT_FALSE(box.addArt(art2)); // should fail (too big)
    EXPECT_EQ(box.getContents().size(), 1); // only one art piece should be in the box
}   

TEST (BoxTest, getTotalWeight) {
    // Arrange
    Box box = Box::makeStandardBox();

    Art art1(1, "0", ACOUSTIC_PANEL, 24, 36, GLAZING_NONE, "", NONE); // 4 lbs
    Art art2(2, "0", ACOUSTIC_PANEL_FRAMED, 24, 36, GLAZING_NONE, "", NONE); // 4 lbs
    Art art3(3, "0", CANVAS_FRAMED, 24, 36, GLAZING_ACRYLIC, "", NONE); // 9 lbs

    // Act
    box.addArt(art1);
    box.addArt(art2);
    box.addArt(art3);
    int totalWeight = box.getTotalWeight();

    // Assert
    EXPECT_EQ(totalWeight, 17); // Total weight should be 4 + 4 + 9 = 17 lbs
}


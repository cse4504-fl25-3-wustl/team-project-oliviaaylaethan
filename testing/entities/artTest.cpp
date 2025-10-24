#include <gtest/gtest.h>
#include "../../src/entities/art.h"

// * Art
//       * getWeight
//       * needsCustomShipping


TEST(ArtTest, getWeight) {
    // Arrange
    Art art1(1, "0", ACOUSTIC_PANEL, 24, 36, GLAZING_NONE, "", NONE); // 3.28
    Art art2(2, "0", ACOUSTIC_PANEL_FRAMED, 24, 36, GLAZING_NONE, "", NONE); // 3.20
    Art art3(3, "0", PAPER_PRINT_FRAMED, 24, 36, GLAZING_ACRYLIC, "", NONE); // 8.12
    Art art4(4, "0", PAPER_PRINT_FRAMED, 24, 36, GLAZING_GLASS, "", NONE); // 8.47
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

TEST(ArtTest, needsCustomPackaging) {        // Anything that exceeds 43.5" in BOTH directions would require custom packaging.
    // Arrange
    Art art1(1, "0", ACOUSTIC_PANEL, 33, 36, GLAZING_NONE, "", NONE); // false
    Art art2(2, "0", CANVAS_FRAMED, 43.5, 43.5, GLAZING_NONE, "", NONE); // false
    Art art3(3, "0", CANVAS_GALLERY, 44, 36, GLAZING_NONE, "", NONE); // false
    Art art4(4, "0", MIRROR, 24, 45, GLAZING_NONE, "", NONE); // false
    Art art5(5, "0", PATIENT_BOARD, 43.5, 36, GLAZING_NONE, "", NONE); // false
    Art art6(5, "0", PATIENT_BOARD, 44, 45, GLAZING_NONE, "", NONE); // true

    // Act & Assert
    EXPECT_FALSE(art1.needsCustomPackaging());
    EXPECT_FALSE(art2.needsCustomPackaging());
    EXPECT_FALSE(art3.needsCustomPackaging());
    EXPECT_FALSE(art4.needsCustomPackaging());
    EXPECT_FALSE(art5.needsCustomPackaging());
    EXPECT_TRUE(art6.needsCustomPackaging());
}
#include <gtest/gtest.h>
#include "../src/interactors/packingInteractor.h"

class PackingInteractorTest : public testing::Test {
    protected:
        PackingInteractorTest() {
        }

        void SetUp() override {
            Requirements requirements;
            requirements.setJobSiteLocation("St. Louis");
            requirements.setClientName("Doctor's Office");
            requirements.setAcceptsPallets("Y");
            requirements.setAcceptsCrates("N");

            // Input 1
            std::vector<Art> artPieces1;
            for (int i = 0; i < 49; i++) {
                artPieces1.push_back(Art(1,"1",GLASS,33,43,GLAZING_GLASS,"",NONE));
            }
            artPieces1.push_back(Art(2,"2",GLASS,34,46,GLAZING_GLASS,"",NONE));
            artPieces1.push_back(Art(2,"2",GLASS,34,46,GLAZING_GLASS,"",NONE));
            artPieces1.push_back(Art(3,"3",GLASS,32,56,GLAZING_GLASS,"",NONE));
            for (int i = 0; i < 3; i++) {
                artPieces1.push_back(Art(4,"4",GLASS,32,48,GLAZING_GLASS,"",NONE));
            }

            request1_ = Request(artPieces1, requirements);
            interactor1_ = PackingInteractor();
            response1_ = std::make_unique<Response>(interactor1_.packAllArt(request1_));
            boxes1_ = interactor1_.getBoxes();
            pallets1_ = interactor1_.getPallets();

            // Input 2
            std::vector<Art> artPieces2;
            for (int i = 0; i < 70; i++) {
                artPieces2.push_back(Art(1,"1",GLASS,36,44,GLAZING_GLASS,"",NONE));
            }
            request2_ = Request(artPieces2, requirements);
            interactor2_ = PackingInteractor();
            response2_ = std::make_unique<Response>(interactor2_.packAllArt(request2_));
            boxes2_ = interactor2_.getBoxes();
            pallets2_ = interactor2_.getPallets();
            
            // Input 3
            std::vector<Art> artPieces3;
            for (int i = 0; i < 11; i++) {
                artPieces3.push_back(Art(1,"1",GLASS,33,43,GLAZING_GLASS,"",NONE));
            }
            artPieces3.push_back(Art(2,"2",GLASS,34,46,GLAZING_GLASS,"",NONE));
            artPieces3.push_back(Art(3,"3",GLASS,34,46,GLAZING_GLASS,"",NONE));

            request3_ = Request(artPieces3, requirements);
            interactor3_ = PackingInteractor();
            response3_ = std::make_unique<Response>(interactor3_.packAllArt(request3_));
            boxes3_ = interactor3_.getBoxes();
            pallets3_ = interactor3_.getPallets();
        };

        Request request1_;
        Request request2_;
        Request request3_;

        PackingInteractor interactor1_;
        PackingInteractor interactor2_;
        PackingInteractor interactor3_;

        std::unique_ptr<Response> response1_;
        std::unique_ptr<Response> response2_;
        std::unique_ptr<Response> response3_;

        std::vector<Box> boxes1_;
        std::vector<Box> boxes2_;
        std::vector<Box> boxes3_;

        std::vector<ShippingContainer> pallets1_;
        std::vector<ShippingContainer> pallets2_;
        std::vector<ShippingContainer> pallets3_;
};

// Check that request art pieces have the correct number
TEST_F(PackingInteractorTest, Initialization) {
    EXPECT_EQ(55, request1_.getArtPieces().size());
    EXPECT_EQ(70, request2_.getArtPieces().size());
    EXPECT_EQ(13, request3_.getArtPieces().size());
}

// Check number of boxes
TEST_F(PackingInteractorTest, BoxNum) {
    EXPECT_EQ(10, interactor1_.getBoxes().size());
    EXPECT_EQ(12, interactor2_.getBoxes().size());
    EXPECT_EQ(3, interactor3_.getBoxes().size());
}

// Check number of pallets
TEST_F(PackingInteractorTest, PalletNum) {
    EXPECT_EQ(2, interactor1_.getPallets().size());
    EXPECT_EQ(3, interactor2_.getPallets().size());
    EXPECT_EQ(1, interactor3_.getPallets().size());
}

// Check number of crates
TEST_F(PackingInteractorTest, CrateNum) {
    EXPECT_EQ(0, interactor1_.getCrates().size());
    EXPECT_EQ(0, interactor2_.getCrates().size());
    EXPECT_EQ(0, interactor3_.getCrates().size());
}

// Check for correct box sizes
TEST_F(PackingInteractorTest, BoxSize) {
    int boxes1_standard = 0;
    int boxes1_large = 0;
    for (size_t i = 0; i < boxes1_.size(); i++) {
        if (boxes1_[i].getBoxType() == STANDARD_BOX) boxes1_standard++;
        else if (boxes1_[i].getBoxType() == LARGE_BOX) boxes1_large++;
    }
    EXPECT_EQ(10, boxes1_standard);
    EXPECT_EQ(0, boxes1_large);
    
    int boxes2_standard = 0;
    int boxes2_large = 0;
    for (size_t i = 0; i < boxes2_.size(); i++) {
        if (boxes2_[i].getBoxType() == STANDARD_BOX) boxes2_standard++;
        else if (boxes2_[i].getBoxType() == LARGE_BOX) boxes2_large++;
    }
    EXPECT_EQ(12, boxes2_standard);
    EXPECT_EQ(0, boxes2_large);

    int boxes3_standard = 0;
    int boxes3_large = 0;
    for (size_t i = 0; i < boxes3_.size(); i++) {
        if (boxes3_[i].getBoxType() == STANDARD_BOX) boxes3_standard++;
        else if (boxes3_[i].getBoxType() == LARGE_BOX) boxes3_large++;
    }
    EXPECT_EQ(3, boxes3_standard);
    EXPECT_EQ(0, boxes3_large);
}

// Check that the pallets are the correct sizes
TEST_F(PackingInteractorTest, PalletSize) {
    int pallets1_standard = 0;
    int pallets1_glass_small = 0;
    int pallets1_oversize = 0;
    for (size_t i = 0; i < pallets1_.size(); i++) {
        if (pallets1_[i].getShippingContainerType() == STANDARD_PALLET) pallets1_standard++;
        else if (pallets1_[i].getShippingContainerType() == GLASS_SMALL_PALLET) pallets1_glass_small++;
        else if (pallets1_[i].getShippingContainerType() == OVERSIZE_PALLET) pallets1_oversize++;
    }
    EXPECT_EQ(0, pallets1_standard);
    EXPECT_EQ(0, pallets1_glass_small);
    EXPECT_EQ(2, pallets1_oversize);
    
    int pallets2_standard = 0;
    int pallets2_glass_small = 0;
    int pallets2_oversize = 0;
    for (size_t i = 0; i < pallets2_.size(); i++) {
        if (pallets2_[i].getShippingContainerType() == STANDARD_PALLET) pallets2_standard++;
        else if (pallets2_[i].getShippingContainerType() == GLASS_SMALL_PALLET) pallets2_glass_small++;
        else if (pallets2_[i].getShippingContainerType() == OVERSIZE_PALLET) pallets2_oversize++;
    }
    EXPECT_EQ(3, pallets2_standard);
    EXPECT_EQ(0, pallets2_glass_small);
    EXPECT_EQ(0, pallets2_oversize);

    int pallets3_standard = 0;
    int pallets3_glass_small = 0;
    int pallets3_oversize = 0;
    for (size_t i = 0; i < pallets3_.size(); i++) {
        if (pallets3_[i].getShippingContainerType() == STANDARD_PALLET) pallets3_standard++;
        else if (pallets3_[i].getShippingContainerType() == GLASS_SMALL_PALLET) pallets3_glass_small++;
        else if (pallets3_[i].getShippingContainerType() == OVERSIZE_PALLET) pallets3_oversize++;
    }
    EXPECT_EQ(1, pallets3_standard);
    EXPECT_EQ(0, pallets3_glass_small);
    EXPECT_EQ(0, pallets3_oversize);
} 

// Check that each box contains the correct number of items
TEST_F(PackingInteractorTest, BoxContent) {
    for (size_t i = 0; i < boxes1_.size(); i++) {
        if (i == boxes1_.size() - 1) {
            EXPECT_EQ(1, boxes1_[i].getContents().size());
        } else {
            EXPECT_EQ(6, boxes1_[i].getContents().size());
        }
    }

    for (size_t i = 0; i < boxes2_.size(); i++) {
        if (i == boxes2_.size() - 1) {
            EXPECT_EQ(4, boxes2_[i].getContents().size());
        } else {
            EXPECT_EQ(6, boxes2_[i].getContents().size());
        }
    }

    for (size_t i = 0; i < boxes3_.size(); i++) {
        if (i == boxes3_.size() - 1) {
            EXPECT_EQ(1, boxes3_[i].getContents().size());
        } else {
            EXPECT_EQ(6, boxes3_[i].getContents().size());
        }
    }
}

// Check that each pallet contains the correct number of items
TEST_F(PackingInteractorTest, PalletContent) {
    for (size_t i = 0; i < pallets1_.size(); i++) {
        EXPECT_EQ(5, pallets1_[i].getContents().size());
    }

    for (size_t i = 0; i < pallets2_.size(); i++) {
        EXPECT_EQ(4, pallets2_[i].getContents().size());
    }

    for (size_t i = 0; i < pallets3_.size(); i++) {
        EXPECT_EQ(3, pallets3_[i].getContents().size());
    }
}

// Check total artwork weights
TEST_F(PackingInteractorTest, ArtworkWeight) {
    int totalWeight = 0;

    for (size_t i = 0; i < boxes1_.size(); i++) {
        totalWeight += boxes1_[i].getTotalWeight();
    }
    EXPECT_EQ(784, totalWeight);

    totalWeight = 0;
    for (size_t i = 0; i < boxes2_.size(); i++) {
        totalWeight += boxes2_[i].getTotalWeight();
    }
    EXPECT_EQ(1120, totalWeight);

    totalWeight = 0;
    for (size_t i = 0; i < boxes3_.size(); i++) {
        totalWeight += boxes3_[i].getTotalWeight();
    }
    EXPECT_EQ(187, totalWeight);
}

// Check total pallet weight
TEST_F(PackingInteractorTest, PalletWeight) {
    int totalWeight = 0;

    for (size_t i = 0; i < pallets1_.size(); i++) {
        totalWeight += pallets1_[i].getTareWeight();
    }
    EXPECT_EQ(150, totalWeight);

    totalWeight = 0;
    for (size_t i = 0; i < pallets2_.size(); i++) {
        totalWeight += pallets2_[i].getTareWeight();
    }
    EXPECT_EQ(180, totalWeight);

    totalWeight = 0;
    for (size_t i = 0; i < pallets3_.size(); i++) {
        totalWeight += pallets3_[i].getTareWeight();
    }
    EXPECT_EQ(60, totalWeight);
}

// Check total weight
TEST_F(PackingInteractorTest, TotalWeight) {
    int totalWeight = 0;
    
    for (size_t i = 0; i < pallets1_.size(); i++) {
        totalWeight += pallets1_[i].getTareWeight();
        std::vector<Box> boxes = pallets1_[i].getContents();
        for (size_t j = 0; j < boxes.size(); j++) {
            totalWeight += boxes[j].getTotalWeight();
        }
    }
    EXPECT_EQ(934, totalWeight);

    totalWeight = 0;
    for (size_t i = 0; i < pallets2_.size(); i++) {
        totalWeight += pallets2_[i].getTareWeight();
        std::vector<Box> boxes = pallets2_[i].getContents();
        for (size_t j = 0; j < boxes.size(); j++) {
            totalWeight += boxes[j].getTotalWeight();
        }
    }
    EXPECT_EQ(1300, totalWeight);

    totalWeight = 0;
    for (size_t i = 0; i < pallets3_.size(); i++) {
        totalWeight += pallets3_[i].getTareWeight();
        std::vector<Box> boxes = pallets3_[i].getContents();
        for (size_t j = 0; j < boxes.size(); j++) {
            totalWeight += boxes[j].getTotalWeight();
        }
    }
    EXPECT_EQ(1300, totalWeight);
}

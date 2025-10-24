#include "responseTest.h"

// Test fixture for ShippingContainer tests
    void ResponseTest::checkOversizedMatch(std::vector<Art> oversized, std::multiset<std::tuple<float, float>> dimensionSet) {
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
    
    void ResponseTest::SetUp() {
        // Create a standard pallet for testing
        testDataPath = std::string(TEST_DATA_PATH) + "/inputsFeature1/"; // Path to test data directory
        requirementsFilePath = testDataPath + "Site_requirements.csv";
        inputPrefix = "Input";
    }

    Response ResponseTest::generateResponse(std::string dataInput, std::string requirements) {
        Request request = parser.parseFiles(dataInput, requirements);

        // check that the file paths are correct (all the other tests will fail if they aren't)
        EXPECT_TRUE(parser.isValidFile(dataInput));
        EXPECT_TRUE(parser.isValidFile(requirements));
    
        PackingInteractor packingInteractor;
        return packingInteractor.packAllArt(request);
    }
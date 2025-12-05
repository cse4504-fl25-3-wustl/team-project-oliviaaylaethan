# team-project-oliviaaylaethan
team-project-oliviaaylaethan created by GitHub Classroom

# Developer Guide

### Build and Run Instructions

Using GitHub releases? Download the appropriate file, then skip to step 5!

1. **Install Prerequisites**

   Ensure that you have CMake and a C++ compiler installed.

   For CMake on Mac, run:
   ```bash
   brew install cmake
   ```

   For CMake on Windows, download from [cmake.org](https://cmake.org/).

   All other prerequisites will be taken care of by CMake. When you first run CMake, it may take a while to download WxWidgets and other libraries. This is normal!

2. **Create a Build Directory**

   Make a new directory for the build files that CMake generates (this directory name is included in .gitignore):
   ```bash
   mkdir build
   cd build
   ```

3. **Generate Build Files**

   Run CMake within the same directory to generate the build system:
   ```bash
   cmake ..
   ```

   Note: On Windows, use powershell.

4. **Build the Project**

   Use the generated build system to compile the project and create an executable:
   ```bash
   cmake --build .
   cpack
   ```

5. **Run the Program**

   **If downloading from GitHub Actions:**
   
   Download the appropriate file. 
   
   - **For Windows**, run the installer.
   
   - **For Mac**, open the .dmg and drag and drop the PackingApp to the Applications folder. Run this command as well to override security protections:
      ```bash
      xattr -cr /Applications/PackingApp.app
      ```

   Then, open the program.

   **If building locally:**

   After building, an executable named `main` on MacOS, or `main.exe` on Windows, will be created in the `build` directory. Run it with this command on Mac:

   (hasCrates should either be a `Y` or `N` if you would like the art pieces to be packed with or without crates)
   ```bash
   ./main <art_data_file.csv> <hasCrates> optional:<output_file_path>
   ```
   or this command on Windows:
   ```bash
   Debug/main.exe <art_data_file.csv> <hasCrates> optional:<output_file_path>
   ```

   Note: The program takes input files as absolute paths.

   Run GUI on Mac:
   ```bash
   ./packingapp
   ```

   Run GUI on Windows:
   ```bash
   Debug/packingapp.exe
   ```


6. **Test the Program**

   Run all tests with this command in the `build` directory:
   ```bash
   ctest
   ```

   Run expected outputs vs actual outputs test (Feature 2) for MacOS:
   ```bash
    ./feature2_main_tests
    ```

   Compare expected outputs to actual outputs with a bash script in the main directory. Include an optional -f flag to see only the failed tests:
   ```bash
    ./run_all_tests.sh <-f>
    ```

   If this doesn't work, you may need to run the following to give it proper permissions:
   ```bash
    chmod +x run_all_tests.sh
    ```
    
   If on Windows, you may need to also run this command to convert the file's line endings:
   ```bash
   dos2unix run_all_tests.sh
   ```

   NOTE: for the tests that require csv inputs, the csv file paths are specified by TEST_DATA_PATH, which is defined in CMakeLists.txt.

   If vscode says TEST_DATA_PATH doesn't exist, add this to your .vscode/c_cpp_properties.json file:
   ```
   "defines": [
       // ... other definitions
       "TEST_DATA_PATH=\"${CMAKE_SOURCE_DIR}/testing/testdata\""
   ],
   ```

# Our Packing Strategies:
- Mixed mediums **CAN** go in the same box
- Pack by depth
   - Each standard box can hold:
      - 6 framed prints
      OR
      - 4 canvas/acoustic panels
      OR
      - a mix based on depth (see below)
   - Each large box can hold:
      - 7 framed prints
      OR
      - 4 canvas/acoustic panels
      OR
      - a mix based on depth (see below)
   - Assumed depths:
      - Based on stated capacity of a standard box (depth of 11")
      - Framed prints = 11" / 6 pieces = 1.833" deep each
      - Canvas/acoustic = 11" / 4 pieces = **2.75"** deep each
      - NOTE: THE INSTRUCTOR-WRITTEN TEST CASES SEEM TO ASSUME A CANVAS DEPTH OF **2.5"**
   <br>
- About custom pieces:
   - Is custom if:
      - BOTH dimensions > 43.5" and/or
      - EITHER dimension > 88"
   - Clarifying examples of what is NOT custom:
      - 75 x 40 is not custom
      - if only ONE dimension exceeds 43.5, do NOT need custom
      - 43.5 x 88 STILL FITS and does NOT need custom packing
   - REASON: 
      - Boxes can be telescoped to a max height of 88"
      - As long as at least ONE dimension of an art piece is 36.5" or less, it will fit in a standard size box
         - Standard box starting dimensions: 37 x 11 x 31
         - Standard box telescoped max dimensions: 37 x 11 x **88**
      - As long as at least ONE dimension of an art piece is 43.5" or less, it will fit in a large size box
         - Large box dimensions: 44 x 13 x 48
         - Large box telescoped max dimensions: 44 x 13 x **88**
   <br>
- Flagging oversize pieces:
   - Anything that exceeds (inclusive) 44" in EITHER direction is considered oversized for installation purposes (because it needs extra support hardware)
   - Examples:
      - 30 x 43.9 is NOT oversize
      - 30 x 44 IS oversize
      - 30 x 44.1 IS oversize

# Test cases that don't apply to us (or are incorrect):
- ## test_cases/stress_tests/strictest_constraint directory
   - Our strategy is to pack by depth, which conflicts with the strictest constraint strategy
   - Example: packing 2 canvas + 4 prints into large boxes:
      - Strictest constraint:
         - canvases can fit 4 per large box
         - prints can fit 6 per large box
         - 4 < 6, so assume only 4 items can fit in a mixed medium box
         - needs 2 boxes
      - Pack by depth:
         - Framed prints = 11" / 6 pieces = 1.833" deep each
         - Canvas/acoustic = 11" / 4 pieces = 2.75" deep each
         - 2 x 2.75" + 4 x 1.833 = 12.832 < 13"
         - only need 1 box

- ## test_cases/stress_tests/no_mixed_medium_in_same_box directory
   - Our strategy is to pack by depth, which conflicts with this strategy
   - Our strategy allows putting different material types in the same box
      - Example: you could put paper prints and canvases in the same box
   - This strategy strictly requires that each material get its own box

- ## test_cases/stress_tests/pack_by_depth/test1/input.csv
   - EXPECTED:
      - "large_box_count": 40
      - "oversized_pallet_count": 2
      - "standard_pallet_count": 22
      - "final_shipment_weight": 6113
      - "total_packaging_weight": 1470

   - SHOULD BE:
      - "large_box_count": 39
      - "oversized_pallet_count": 8
      - "standard_pallet_count": 14
      - "final_shipment_weight": 6083
      - "total_packaging_weight": 1440

   - EXPLANATION:
      - "large_box_count"
         - This test case measured depth by fractions instead of inches
         - Test Case's Assumption: these numbers are the same, regardless of whether box is standard or large
            - 1/6 box each:
               - Paper Print – Framed
            - 1/4 box each:
               - Canvas – Gallery
               - Canvas – Float Frame
               - Acoustic panel
               - Acoustic panel – Framed
         - Truth:
            - A large box can really hold 7 framed prints, and the mixed medium combinations are a lot more flexible.
            - Using inches instead of fractions, these combinations can fit in the same large box (but wouldn't work with the given fractions):
               - depths (see above for derivation):
                  - print = 1.833"
                  - canvas/acoustic = 2.75"
                  - large box = 13"
               - new combinations
                  - 7 prints
                     - 12.831 < 13"
                  - 1 canvas + 5 prints
                     - 11.915 < 13"
                  - 2 canvas + 4 prints
                     - 12.832 < 13"
                  - 4 canvas + 1 prints
                     - 12.833 < 13"
         - Since you can fit more art per large box, you don't need one of the proposed large boxes
      - pallets/weight
         - oversize: 75 lbs
         - standard: 60 lbs
         - 2 oversize + 22 standard pallets = 2x75 + 22x60 = 1470 lbs
         - 8 oversize + 14 standard pallets = 8x75 + 14x60 = 1440 lbs
         - without the unnecessary final large box, you can do this arrangement that lowers the packaging weight (and final weight) by 30 lbs



- ## test_cases/stress_tests/pack_by_depth/test4/input.csv
   - EXPECTED:
      - "final_shipment_weight": 8000
      - "large_box_count": 84
      - "oversized_pallet_count": 1
      - "standard_box_count": 17
      - "standard_pallet_count": 31
      - "total_packaging_weight": 1935

   - SHOULD BE:
      - "final_shipment_weight": 7940
      - "large_box_count": 82
      - "oversized_pallet_count": 2
      - "standard_box_count": 16
      - "standard_pallet_count": 29
      - "total_packaging_weight": 1890
    
   - EXPLANATION:
      - This test case assumed only 6 paper prints fit in a large box, while we fit 7 per large box
      - Large boxes:
         - 107 framed paper prints:
            - 7 per large box
            - 107 / 7 = 15.286 -> 15 full boxes. 16th box has 2 framed prints
         - 113 + 151 = 264 acoustic panels
            - 4 per large box
            - 264 / 4 = 66 full boxes
         - Total large boxes: 16 + 66 = 82
      - Standard boxes:
         - 101 framed paper prints
         - 6 per standard box
         - 5 of these can go in the 16th large paper print box
         - Leaves us with 101-5 = 96 prints
         - 96 / 6 = 16 standard boxes
      - Pallets:
         - Need to pack 82 large boxes and 16 standard boxes
         - Standard pallet can hold 4 standard or 3 large boxes
         - Oversize pallet can hold 5 standard or 3 large boxes
         - 82/3 = 27.33 standard pallets for large boxes
            - Can fit 2 standard boxes onto this 1/3 full pallet
            - Left with 14 standard boxes
            - 2 oversize pallets (5 each) and 1 standard pallet (4 each) minimizes weight
         - Standard pallets = 28 + 1 = 29
         - Oversize pallets = 2
      - Weight:
         - Packaging weight = 29x60 + 2x75 = 1890
         - Final shipment weight = 1890 + 6050 = 7940



- ## test_cases/stress_tests/pack_by_depth/test6/input.csv
   - EXPECTED:
      - "standard_size_pieces": 100
      - "standard_pallet_count": 9
      - "oversized_pallet_count": 5

   - SHOULD BE:
      - "standard_size_pieces": 0
      - "standard_pallet_count": 11
      - "oversized_pallet_count": 3

   - EXPLANATION:
      - standard_size_pieces
         - Pieces that are >= 44" (INclusive) in any dimension are marked as oversized
            - We specifically wrote "inclusive" in our notes, so this should be what the client wants
         - Test assumes pieces that are > 44" (EXclusive) in any dimension are marked as oversized
         - We mark 36.6 x 44 pieces as oversized, this test doesn't
      - pallets
         - oversize: 75 lbs
         - standard: 60 lbs
         - 5 oversize +  9 standard pallets = 5x75 +  9x60 = 915 lbs
         - 3 oversize + 11 standard pallets = 3x75 + 11x60 = 885 lbs
         - This test shows the correct final weight but incorrect number of each type of pallet in the json



- ## test_cases/stress_tests/pack_by_depth/test8/stress_test.csv
   - This test assumed that canvases (or 4PerBox) are **2.5"** wide and paper prints (or 6PerBox) are 1.83" wide
   - We calculated material depth like so:
      - Framed prints = 11" / 6 pieces = 1.833" deep each
      - Canvas/acoustic = 11" / 4 pieces = **2.75"** deep each
   - Since we assumed a greater depth, we needed 9 more large boxes to pack the same artwork, which:
      - increased our number of standard pallets by 3 (3 large boxes per standard pallet)
      - increased our packaging weight by 180 lbs (3 x 60 lbs)
      - increased our final shipment weight by that same 180 lbs



- ## /stress_tests/pack_by_depth/test9/input.csv
   - This test did NOT allow mixing mediums within the same box
   - From their explanation:
      - Packing for standard boxes:
         - 60 paper prints: 6 pieces go in a box, so we have 60/6 = 10 boxes with no space left.
         - 70 canvas: 4 pieces go in a box, so we have 70/4 = 17.5 --> 18 boxes. None of these pieces will fit in with the paper print boxes.
      - Packing for large boxes:
         - 50 paper prints: 7 pieces go in a box, so we have 50/7 = 7.14 --> 8 boxes.
         - 70 canvas: 4 pieces go in a box, so we have 70/4 = 17.5 --> 18 boxes. None of these pieces fit in with the paper print boxes.
      - They had 28 standard boxes and 26 large boxes

   - How we would pack these (allowing mixed mediums, packing large boxes first):
      - Packing for large boxes:
         - 50 paper prints: 7 pieces per large box, so we have 50/7 = 7.14 --> 7 full boxes
            - 1 box that's 1.833" / 13" full
         - 70 canvas: 4 pieces per box
            - 4 canvas can fit in that remaining box (1.833" + 4 x 2.75" = 12.833 < 13")
            - 66 remaining canvases
            - 66/4 = 16.5 boxes
         - Large boxes: 8 + 16 = 24 full boxes, 1 at 5.5" / 13" capacity
      - Packing for standard boxes:
         - 60 paper prints: 6 pieces go in a box, so we have 60/6 = 10 full boxes
         - 70 canvas: 4 pieces per box
            - 2 of these can go in the half-full large box
            - 68 remaining
            - 68/4 = 17 standard boxes
         - Standard boxes: 10 + 17 = 27
      - We have 27 standard boxes and 25 large boxes
      - These can fit on 9 standard pallets and 5 oversize pallets
         - Gives a packaging weight of 915 and final shipment weight of 4175

   - Other mismatch:
      - EXPECTED:
         - "standard_size_pieces": 130
      - SHOULD BE:
         - "standard_size_pieces": 250
      - EXPLANATION:
         - A piece must be >= 44" in at least one dimension to count as oversize
         - Out of 470 pieces, only the final 2 lines had art with a dimension >=44"
            - 48" x 50" (quantity 100)
            - 48" x 50" (quantity 120)
         - This means every other piece of art is qualifies as standard_size_pieces
            - 470 - 100 - 120 = 250
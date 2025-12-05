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
## Student-written stress tests:
- test_cases/stress_tests/pack_by_depth/test1/input.csv
   - test assumes **6** framed prints per large box
   - we have **7** framed prints per large box
- test_cases/stress_tests/no_mixed_medium_in_same_box directory
   - tests assumes that each medium gets its own box
   - we allow packing different mediums into the same box
## Stress_tests_redo:
- test_cases/stress_tests/all/test2/expected_output.json
   - EXPECTS: "custom_piece_count": 200
   - CORRECT: "custom_piece_count": 0
   - Reason (see "About custom pieces" for more information):
      - As long as at least ONE dimension of an art piece is 36.5" or less, it will fit in a standard size box (assuming the other dimension is <= 88")
      - The only piece sizes given for this input are 33x43 and 32x56
      - 33 <= 36.5"
      - 32 <= 36.5"
      - 43 <= 88"
      - 56 <= 88"
   - OTHER TYPOS:
      - README calls for "67 standard boxes" and "11 large pallets"
      - EXPECTED OUTPUT: 
         - "large_box_count": 11
         - "oversized_pallet_count": 0
      - SHOULD BE:
         - "large_box_count": 0
         - "oversized_pallet_count": 11

- test_cases/stress_tests/pack_by_depth/test6/input.csv
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



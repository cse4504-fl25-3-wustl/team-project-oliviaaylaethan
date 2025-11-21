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

# Feature 1 Work Assignment
* **Olivia** - Response
* **Ayla** - Request
* **Ethan** - Interactor

# Testing Assignment
* **Olivia**
   * Packing Interactor
      * packAllArt
* **Ayla**
   * Art
      * getWeight
      * needsCustomShipping
   * Box
      * fitsArt
      * addArt
      * getTotalWeight
* **Ethan**
   * Shipping Container
      * calculateContainerHeight
      * addBox
   * CSV Parser
      * parseFiles
    
# Feature 2 Work Plan

Workflow: all of us will commit directly to the feature2 branch

Responsibilities:
 
* **Olivia**
   * change input to accept command line args
   * change output to json
* **Ethan**
   * add test cases from our class github repo
* **Ayla**
   * update code to pass new test cases


# Feature 3 Work Plan

This week:
- **Olivia:** output stuff
- **Ethan:** git bash testing repo in separate location
- **Ayla:** GUI improvements

General TODOs:
- get more GitHub minutes
- fix crate stuff
- check abt mirrors
- RUN TEST CASES and check that depth-based test cases pass
- email
- collapsible or tabs for different summaries
- clear output from last run
- fix bugs
- make sure errors print out to user

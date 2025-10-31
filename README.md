# team-project-oliviaaylaethan
team-project-oliviaaylaethan created by GitHub Classroom

# Developer Guide
Ensure that you have CMake and a C++ compiler installed.

For CMake on Mac, run:
```bash
brew install cmake
```

For CMake on Windows, download from [cmake.org](https://cmake.org/).

### Build and Run Instructions

1. **Create a Build Directory**

   Make a new directory for the build files that CMake generates (this directory name is included in .gitignore):
   ```bash
   mkdir build
   cd build
   ```

2. **Generate Build Files**

   Run CMake to generate the build system:
   ```bash
   cmake ..
   ```

3. **Build the Project**

   Use the generated build system to compile the project:
   ```bash
   cmake --build .
   ```

4. **Run the Program**

   After building, an executable named `main` on MacOS, or `main.exe` on Windows, will be created in the `build` directory. Run it with this command on Mac:
   ```bash
   ./main <art_data_file.csv> <requirements_file.csv> optional:<output_file_path>
   ```
   or this command on Windows:
   ```bash
   Debug/main.exe <art_data_file.csv> <requirements_file.csv> optional:<output_file_path>
   ```

   Note: The program takes input files as absolute paths.

5. **Test the Program**

   Run all tests with this command in the `build` directory:
   ```bash
   ctest
   ```

   Run expected outputs vs actual outputs test (Feature 2) for MacOS:
   ```bash
    ./feature2_main_tests
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

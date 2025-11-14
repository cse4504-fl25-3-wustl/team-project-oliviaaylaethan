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

1. **Install Prerequisites**

   Install wxwidgets first.

   On Mac:
   ```bash
   brew install wxwidgets
   ```

   On Windows:
   https://wxwidgets.org/downloads/#v3.3.1_msw
   
   Download Windows Binaries:
   - Visual Studio
      - Header files
      - Development files

   Unzip files and move to this folder:
      C:\wxWidgets

   Inside should include:
      - lib
      - licence.txt
      - wxwidgets.props
      - build
      - include

2. **Create a Build Directory**

   Make a new directory for the build files that CMake generates (this directory name is included in .gitignore):
   ```bash
   mkdir build
   cd build
   ```

3. **Generate Build Files**

   Run CMake to generate the build system:
   ```bash
   cmake ..
   ```

   Note: On Windows, use powershell.

4. **Build the Project**

   Use the generated build system to compile the project:
   ```bash
   cmake --build .
   ```

   To run GUI on Windows, add files from C:\wxWidgets\lib\vc14x_x64_dll below to build/Debug:
   (All of these files are debug. 331ud_. Release files are 331u_ with no d and should NOT be included)
   - wxbase331ud_net_vc14x_x64.dll
   - wxbase331ud_vc14x_x64.dll
   - wxbase331ud_xml_vc14x_x64.dll
   - wxmsw331ud_adv_vc14x_x64.dll
   - wxmsw331ud_aui_vc14x_x64.dll
   - wxmsw331ud_core_vc14x_x64.dll
   - wxmsw331ud_gl_vc14x_x64.dll
   - wxmsw331ud_html_vc14x_x64.dll
   - wxmsw331ud_media_vc14x_x64.dll
   - wxmsw331ud_propgrid_vc14x_x64.dll
   - wxmsw331ud_qa_vc14x_x64.dll
   - wxmsw331ud_ribbon_vc14x_x64.dll
   - wxmsw331ud_richtext_vc14x_x64.dll
   - wxmsw331ud_stc_vc14x_x64.dll
   - wxmsw331ud_webview_vc14x_x64.dll
   - wxmsw331ud_xrc_vc14x_x64.dll

5. **Run the Program**

   After building, an executable named `main` on MacOS, or `main.exe` on Windows, will be created in the `build` directory. Run it with this command on Mac:
   ```bash
   ./main <art_data_file.csv> <requirements_file.csv> optional:<output_file_path>
   ```
   or this command on Windows:
   ```bash
   Debug/main.exe <art_data_file.csv> <requirements_file.csv> optional:<output_file_path>
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

   Compare expected outputs to actual outputs with a bash script in the main directory:
   ```bash
    ./run_all_tests.sh
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

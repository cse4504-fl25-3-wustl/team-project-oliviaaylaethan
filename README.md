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
   ./main
   ```
   or this command on Windows:
   ```bash
   Debug/main.exe
   ```

   Note: The program takes input files as absolute paths.

5. **Test the Program**

   Run all tests with this command in the `build` directory:
   ```bash
   ctest
   ```

   Run packing interactor tests with this command in the `build` directory:
   ```bash
   ./packing_interactor_test
   ```


# Feature 1 Work Assignment
* **Olivia** - Response
* **Ayla** - Request
* **Ethan** - Interactor

# Testing Assignment
* ***Olivia**
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

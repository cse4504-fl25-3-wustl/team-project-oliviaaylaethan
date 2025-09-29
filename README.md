# team-project-oliviaaylaethan
team-project-oliviaaylaethan created by GitHub Classroom

# Developer Guide
Ensure that you have g++ installed.

### On Windows:
`g++ -Wall -std=c++17 -I./src src/main.cpp src/interactors/packingInteractor.cpp src/requests/request.cpp src/parser/csvParser.cpp -o main.exe -Wl,-subsystem,console`

### On Mac:
`g++ -Wall -std=c++17 -I./src src/main.cpp src/interactors/packingInteractor.cpp src/requests/request.cpp src/parser/csvParser.cpp -o main`

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

   After building, an executable named `main` on MacOS, or `main.exe` on Windows, will be created in the `build` directory. Run it with:
   ```bash
   ./main
   ```

# Feature 1 Work Assignment
Olivia - Response
Ayla - Request
Ethan - Interactor
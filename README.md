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
   main.exe
   ```

### Backup Manual Compiler Commands
On Windows:
```bash
g++ -Wall -std=c++17 -I./src src/main.cpp src/interactors/packingInteractor.cpp src/requests/request.cpp src/parser/csvParser.cpp -o main.exe -Wl,-subsystem,console
```

On Mac:
```bash
g++ -Wall -std=c++17 -I./src src/main.cpp src/interactors/packingInteractor.cpp src/requests/request.cpp src/parser/csvParser.cpp -o main
```


# Feature 1 Work Assignment
* **Olivia** - Response
* **Ayla** - Request
* **Ethan** - Interactor

# Run response testing on Mac
```bash
g++ -Wall -std=c++20 -I./src \
   src/responses/testing.cpp \
   src/entities/art.cpp \
   src/entities/box.cpp \
   src/entities/pallet.cpp \
   src/entities/crate.cpp \
   src/responses/response.cpp \
   src/responses/boxInfo.cpp \
   src/responses/artInfo.cpp \
   src/responses/palletInfo.cpp \
   src/responses/crateInfo.cpp \
   -o test
```
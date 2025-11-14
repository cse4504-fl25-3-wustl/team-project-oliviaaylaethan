cmake_minimum_required(VERSION 3.28)
project(team_project_oliviaaylaethan)

set(CMAKE_CXX_STANDARD 20)

# ensures FAIL() in test files doesn't incorrectly compile as 0() on Windows
if(WIN32)
    add_compile_definitions(
        WIN32_LEAN_AND_MEAN
        NOMINMAX
    )
endif()


include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/279f8479469d22fa772adb454068f854472e1eb9.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

FetchContent_Declare(
    nlohmann_json
    GIT_REPOSITORY https://github.com/nlohmann/json.git
    GIT_TAG v3.12.0
)
FetchContent_MakeAvailable(nlohmann_json)

# Create static library for source files
add_library(core_lib STATIC
    src/entities/art.cpp
    src/entities/box.cpp
    src/entities/requirements.cpp
    src/entities/shippingContainer.cpp
    src/interactors/packingInteractor.cpp
    src/parser/csvParser.cpp
    src/requests/request.cpp
    src/responses/response.cpp
    src/responses/responseSummary.cpp
    src/responses/artInfo.cpp
    src/responses/boxInfo.cpp
    src/responses/crateInfo.cpp
    src/responses/hardwareInfo.cpp
    src/responses/palletInfo.cpp
    src/gui/estimator.cpp
)

target_include_directories(core_lib PUBLIC
    src/entities
    src/interactors
    src/parser
    src/requests
    src/responses
)

target_link_libraries(core_lib PUBLIC nlohmann_json::nlohmann_json) # Ensure nlohmann_json is linked

# Set the executable name to 'main'
add_executable(
    main
    src/main.cpp
)
target_link_libraries(
    main
    core_lib
)

# --- INSERT STATIC LINKING FLAGS HERE FOR WINDOWS PORTABILITY ---
if(CMAKE_CXX_COMPILER_ID STREQUAL "GNU")
    target_link_options(main PRIVATE
        -static-libgcc
        -static-libstdc++
    )
endif()

# --- MAC STATIC LINKING FLAGS ---
if(CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_link_options(main PRIVATE
        -stdlib=libc++ # Use libc++ standard library
        -static-libclang # Statically link clang runtime
        -static-libstdc++
    )
endif()

enable_testing()

# Define the path to your test data directory so that test csv files are accessible across Windows vs Mac
set(TEST_DATA_PATH "${CMAKE_SOURCE_DIR}/testing/testdata") 

# Pass it to your test executable as a preprocessor macro
add_compile_definitions(TEST_DATA_PATH="${TEST_DATA_PATH}")

add_executable(
  all_tests
  testing/parser/csvParserTest.cpp
  testing/entities/artTest.cpp
  testing/entities/boxTest.cpp
  testing/entities/shippingContainerTest.cpp
  testing/interactors/packingInteractorTest.cpp
  testing/responses/responseTest.cpp
  testing/responses/feature1Test.cpp
)
target_link_libraries(
  all_tests
  core_lib
  GTest::gtest
  GTest::gtest_main
)

add_executable(
    feature2_main_tests
    testing/responses/feature2MainTest.cpp
)
target_link_libraries(
    feature2_main_tests
    core_lib
    GTest::gtest
    GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(all_tests)

# GUI application setup 
find_package(wxWidgets REQUIRED COMPONENTS net core base) 
if(wxWidgets_USE_FILE) # not defined in CONFIG mode 
    include(${wxWidgets_USE_FILE}) 
endif()

# On Windows, build a WIN32 subsystem app (no console, no main())
# On macOS/Linux, build normally.
if (WIN32)
    add_executable(packingapp WIN32
        src/gui/packingapp.cpp
    )
else()
    add_executable(packingapp
        src/gui/packingapp.cpp
    )
endif()

target_link_libraries(packingapp core_lib ${wxWidgets_LIBRARIES}) 
target_include_directories(packingapp PRIVATE src/gui)

# Static linking for packingapp on Windows
if(WIN32 AND MINGW)
    target_link_options(packingapp PRIVATE
        -static-libgcc
        -static-libstdc++
        -static
        -Wl,--subsystem,windows
    )
endif()

# CPack configuration for DragNDrop
if(APPLE)
    set(MACOSX_BUNDLE_ICON_FILE app_icon.icns)
    set_source_files_properties(${CMAKE_SOURCE_DIR}/_deps/app_icon.icns PROPERTIES MACOSX_PACKAGE_LOCATION Resources)

    set_target_properties(packingapp PROPERTIES
        MACOSX_BUNDLE TRUE
        MACOSX_BUNDLE_INFO_PLIST ${CMAKE_SOURCE_DIR}/_deps/Info.plist
    )

    set(CPACK_GENERATOR "DragNDrop")
    set(CPACK_DMG_VOLUME_NAME "${CPACK_PACKAGE_NAME}")
    set(CPACK_DMG_FORMAT "UDBZ") # Compressed DMG

    # Ensure the .app bundle is included in the DMG
    set(CPACK_DMG_CONTENT
        "${CMAKE_BINARY_DIR}/packingapp.app /Applications"
    )
    install(TARGETS packingapp BUNDLE DESTINATION ".")
endif()

# Configure CPack for Windows NSIS installer
if(WIN32)
    install(TARGETS packingapp DESTINATION "bin")
    
    # Install wxWidgets DLLs alongside the executable
    if(DEFINED ENV{wxWidgets_LIB_DIR})
        file(GLOB WXWIDGETS_DLLS "$ENV{wxWidgets_LIB_DIR}/*.dll")
        install(FILES ${WXWIDGETS_DLLS} DESTINATION "bin")
    endif()
    
    # Install MinGW runtime DLLs if needed
    if(MINGW)
        get_filename_component(MINGW_BIN_DIR ${CMAKE_CXX_COMPILER} DIRECTORY)
        install(FILES 
            "${MINGW_BIN_DIR}/libgcc_s_seh-1.dll"
            "${MINGW_BIN_DIR}/libstdc++-6.dll"
            "${MINGW_BIN_DIR}/libwinpthread-1.dll"
            DESTINATION "bin"
            OPTIONAL
        )
    endif()
    
    set(CPACK_PACKAGE_NAME "PackingApp")
    set(CPACK_PACKAGE_VENDOR "Team Olivia Ayla Ethan")
    set(CPACK_PACKAGE_VERSION "1.0.0")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "PackingApp - Art Packing Calculator")
    
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_PACKAGE_FILE_NAME "PackingAppInstaller")
    set(CPACK_NSIS_DISPLAY_NAME "PackingApp")
    set(CPACK_NSIS_PACKAGE_NAME "PackingApp")
    set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\packingapp.exe")
    set(CPACK_NSIS_EXECUTABLES_DIRECTORY "bin")
    
    # Create Start Menu shortcuts
    set(CPACK_NSIS_CREATE_ICONS_EXTRA
        "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\PackingApp.lnk' '$INSTDIR\\\\bin\\\\packingapp.exe'"
    )
    set(CPACK_NSIS_DELETE_ICONS_EXTRA
        "Delete '$SMPROGRAMS\\\\$START_MENU\\\\PackingApp.lnk'"
    )
    
    # Modify PATH to include bin directory
    set(CPACK_NSIS_MODIFY_PATH ON)
endif()

# Include CPack
include(CPack)
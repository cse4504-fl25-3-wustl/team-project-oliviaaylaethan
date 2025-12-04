cmake_minimum_required(VERSION 3.10)
project(team_project_oliviaaylaethan)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include(FetchContent)

FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/279f8479469d22fa772adb454068f854472e1eb9.zip
  DOWNLOAD_EXTRACT_TIMESTAMP TRUE # to fix "policy CMP0135 is not set" warning
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

include(GoogleTest)
gtest_discover_tests(all_tests)

# GUI application setup - fetch wxWidgets

FetchContent_Declare(
    wxWidgets
    GIT_REPOSITORY https://github.com/wxWidgets/wxWidgets.git
    GIT_TAG v3.3.1
    GIT_SHALLOW TRUE
)

# Configure wxWidgets build options - disable everything except what we need
set(wxBUILD_SHARED OFF CACHE BOOL "Build wx libraries as shared libs")
set(wxBUILD_SAMPLES OFF CACHE BOOL "Build wxWidgets samples")
set(wxBUILD_TESTS OFF CACHE BOOL "Build wxWidgets tests")
set(wxBUILD_DEMOS OFF CACHE BOOL "Build wxWidgets demos")
set(wxBUILD_BENCHMARKS OFF CACHE BOOL "Build wxWidgets benchmarks")
set(wxBUILD_INSTALL OFF CACHE BOOL "Do not install wxWidgets")
set(wxUSE_MEDIACTRL OFF CACHE BOOL "Disable media control")
set(wxUSE_WEBVIEW OFF CACHE BOOL "Disable webview")
set(wxUSE_XRC OFF CACHE BOOL "Disable XRC resources")
set(wxUSE_AUI OFF CACHE BOOL "Disable AUI docking")
set(wxUSE_RIBBON OFF CACHE BOOL "Disable ribbon")
set(wxUSE_PROPGRID OFF CACHE BOOL "Disable property grid")
set(wxUSE_RICHTEXT OFF CACHE BOOL "Disable rich text")
set(wxUSE_STC OFF CACHE BOOL "Disable styled text control")
set(wxUSE_GLCANVAS OFF CACHE BOOL "Disable OpenGL canvas")

FetchContent_MakeAvailable(wxWidgets)

# On Windows, build a WIN32 subsystem app (no console, no main())
# On macOS/Linux, build normally.
if (WIN32)
    # Create a resource file for Windows icon
    set(ICON_RC "${CMAKE_BINARY_DIR}/app_icon.rc")
    file(WRITE ${ICON_RC} "IDI_ICON1 ICON \"${CMAKE_SOURCE_DIR}/_deps/app_icon.ico\"\n")
    
    add_executable(packingapp WIN32
        src/gui/packingapp.cpp
        ${ICON_RC}
    )
elseif(APPLE)
    # For macOS, add icon as a resource
    set(MACOSX_ICON_FILE "${CMAKE_SOURCE_DIR}/_deps/app_icon.icns")
    add_executable(packingapp MACOSX_BUNDLE
        src/gui/packingapp.cpp
        ${MACOSX_ICON_FILE}
    )
    set_source_files_properties(${MACOSX_ICON_FILE} PROPERTIES
        MACOSX_PACKAGE_LOCATION Resources
    )
else()
    add_executable(packingapp
        src/gui/packingapp.cpp
    )
endif()

target_link_libraries(packingapp 
    core_lib 
    wx::net 
    wx::core 
    wx::base
) 
target_include_directories(packingapp PRIVATE src/gui)

# Include the resource file for wxWidgets manifest (windows GUI gives warning otherwise)
if(WIN32)
    target_sources(packingapp PRIVATE src/gui/resources.rc)
endif()

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
    
    set_target_properties(packingapp PROPERTIES
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
    
    set(CPACK_PACKAGE_NAME "Packing Estimator")
    set(CPACK_PACKAGE_VENDOR "Team Olivia Ayla Ethan")
    set(CPACK_PACKAGE_VERSION "1.0.0")
    set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Packing Estimator - Art Packing Calculator")
    
    set(CPACK_GENERATOR "NSIS")
    set(CPACK_PACKAGE_FILE_NAME "PackingEstimatorInstaller")
    set(CPACK_NSIS_DISPLAY_NAME "Packing Estimator")
    set(CPACK_NSIS_PACKAGE_NAME "Packing Estimator")
    set(CPACK_NSIS_INSTALLED_ICON_NAME "bin\\\\packingapp.exe")
    set(CPACK_NSIS_EXECUTABLES_DIRECTORY "bin")
    
    # Create Start Menu shortcuts
    set(CPACK_NSIS_CREATE_ICONS_EXTRA
        "CreateShortCut '$SMPROGRAMS\\\\$STARTMENU_FOLDER\\\\Packing Estimator.lnk' '$INSTDIR\\\\bin\\\\packingapp.exe'"
    )
    set(CPACK_NSIS_DELETE_ICONS_EXTRA
        "Delete '$SMPROGRAMS\\\\$START_MENU\\\\Packing Estimator.lnk'"
    )
    
    # Modify PATH to include bin directory
    set(CPACK_NSIS_MODIFY_PATH ON)
endif()

# Include CPack
include(CPack)
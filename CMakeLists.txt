cmake_minimum_required(VERSION 4.0)
project(team_project_oliviaaylaethan)

set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/279f8479469d22fa772adb454068f854472e1eb9.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Create static library for source files
add_library(core_lib
    src/entities/art.cpp
    src/entities/box.cpp
    src/entities/requirements.cpp
    src/entities/shippingContainer.cpp
    src/interactors/packingInteractor.cpp
    src/parser/csvParser.cpp
    src/requests/request.cpp
    src/responses/response.cpp
    src/responses/artInfo.cpp
    src/responses/boxInfo.cpp
    src/responses/crateInfo.cpp
    src/responses/hardwareInfo.cpp
    src/responses/palletInfo.cpp
)

target_include_directories(core_lib PUBLIC
    src/entities
    src/interactors
    src/parser
    src/requests
    src/responses
)

# Set the executable name to 'main'
add_executable(
    main
    src/main.cpp
)
target_link_libraries(
    main
    core_lib
)

enable_testing()

# Art Tests
add_executable(
  art_test
  testing/entities/artTest.cpp
  src/entities/art.cpp
  src/entities/art.h
)
target_link_libraries(
  art_test
  GTest::gtest_main
)

# Shipping Container Tests
add_executable(
  shipping_container_test
  testing/entities/shippingContainerTest.cpp
  src/entities/shippingContainer.cpp
  src/entities/shippingContainer.h
  src/entities/box.cpp
  src/entities/box.h
  src/entities/art.cpp
  src/entities/art.h
)
target_link_libraries(
  shipping_container_test
  GTest::gtest_main
)

# CSV Parser Tests
add_executable(
  csv_parser_test
  testing/parser/csvParserTest.cpp
  src/parser/csvParser.cpp
  src/parser/csvParser.h
  src/entities/art.cpp
  src/entities/art.h
  src/entities/requirements.cpp
  src/entities/requirements.h
  src/requests/request.cpp
  src/requests/request.h
)
target_link_libraries(
  csv_parser_test
  GTest::gtest_main
)

add_executable(
    packing_interactor_test
    testing/packingInteractorTest.cpp
)
target_link_libraries(
    packing_interactor_test
    GTest::gtest_main
    core_lib
)

include(GoogleTest)
gtest_discover_tests(art_test)
gtest_discover_tests(shipping_container_test)
gtest_discover_tests(csv_parser_test)

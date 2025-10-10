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

include_directories(src/entities)
include_directories(src/interactors)
include_directories(src/parser)
include_directories(src/requests)
include_directories(src/responses)

# Set the executable name to 'main'
add_executable(main
        src/entities/art.cpp
        src/entities/art.h
        src/entities/box.cpp
        src/entities/box.h
        src/entities/requirements.cpp
        src/entities/requirements.h
        src/entities/shippingContainer.cpp
        src/entities/shippingContainer.h
        src/interactors/packingInteractor.cpp
        src/interactors/packingInteractor.h
        src/parser/csvParser.cpp
        src/parser/csvParser.h
        src/requests/request.cpp
        src/requests/request.h
        src/responses/response.cpp
        src/responses/response.h
        src/responses/artInfo.cpp
        src/responses/boxInfo.cpp
        src/responses/crateInfo.cpp
        src/responses/hardwareInfo.cpp
        src/responses/palletInfo.cpp
        src/main.cpp)

enable_testing()

add_executable(
  hello_test
  testing/artTest.cpp
)
target_link_libraries(
  hello_test
  GTest::gtest_main
)

include(GoogleTest)
gtest_discover_tests(hello_test)

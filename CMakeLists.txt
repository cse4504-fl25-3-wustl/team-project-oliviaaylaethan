cmake_minimum_required(VERSION 4.0)
project(team_project_oliviaaylaethan)

set(CMAKE_CXX_STANDARD 20)

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

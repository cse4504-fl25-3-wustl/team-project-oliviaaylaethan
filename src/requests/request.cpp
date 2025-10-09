#include "request.h"

Request::Request(std::vector<Art> artPieces, Requirements requirements) {
    artPieces_ = artPieces;
    requirements_ = requirements;
}

Request::Request() {
    artPieces_ = std::vector<Art>();
    requirements_ = Requirements();
}

std::vector<Art> Request::getArtPieces() {
    return artPieces_;
}

Requirements Request::getRequirements() {
    return requirements_;
}
#pragma once
#include <string>
#include <vector>
#include "../entities/art.h"
#include "../entities/requirements.h"

class Request {
private:
    // client information
    Requirements requirements_;
    // art to be packaged
    std::vector<Art> artPieces_;

public:
    Request(std::vector<Art> artPieces, Requirements rules);
    Request();
    std::vector<Art> getArtPieces();
    Requirements getRequirements();
};

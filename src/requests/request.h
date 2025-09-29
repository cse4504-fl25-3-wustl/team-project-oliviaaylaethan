#pragma once
#include <string>
#include <vector>
#include "../entities/art.h"

class Request {
private:
    // client information
    // std::string jobSiteLocation_;
    // std::string clientName_;
    // bool acceptsPallets_;
    // bool acceptsCrates_;
    // bool hasLoadingDock_;
    // bool needsLiftgate_;
    // bool needsInsideDelivery_;
    // std::string serviceType_;
    // art to be packaged
    std::vector<Art> artPieces_;

public:
    Request(std::vector<Art> artPieces);
    Request();

};

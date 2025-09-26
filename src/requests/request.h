#pragma once
#include <string>
#include <vector>
#include "../entities/art.h"

class Request {
    private:
    // client information
    std::string jobSiteLocation;
    std::string clientName;
    bool acceptsPallets;
    bool acceptsCrates;
    bool hasLoadingDock;
    bool needsLiftgate;
    bool needsInsideDelivery;
    std::string serviceType;
    // art to be packaged
    std::vector<Art> artPieces;

    public:
        Request(std::vector<Art> artPieces);

};

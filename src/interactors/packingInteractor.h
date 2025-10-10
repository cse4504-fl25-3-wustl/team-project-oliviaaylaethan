#pragma once
#include "../entities/box.h"
#include "../entities/shippingContainer.h"
#include "../requests/request.h"
#include "../responses/response.h"
#include <vector>
#include <string>

class PackingInteractor {
    private:
        std::vector<Box> boxes_;
        std::vector<ShippingContainer> pallets_;
        std::vector<ShippingContainer> crates_;

    public:
        PackingInteractor();

        Response packAllArt(Request request);

        std::vector<Box> getBoxes();
        std::vector<ShippingContainer> getPallets();
        std::vector<ShippingContainer> getCrates();
};

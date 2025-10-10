#pragma once
#include "../entities/box.h"
#include "../entities/pallet.h"
#include "../entities/crate.h"
#include "../requests/request.h"
#include "../responses/response.h"
#include <vector>
#include <string>

class PackingInteractor {
    private:
        std::vector<Box> boxes_;
        std::vector<Pallet> pallets_;
        std::vector<Crate> crates_;

    public:
        PackingInteractor();

        Response packAllArt(Request request);

        std::vector<Box> getBoxes();
        std::vector<Pallet> getPallets();
        std::vector<Crate> getCrates();
};

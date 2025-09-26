#pragma once
#include "box.h"
#include "pallet.h"
#include "crate.h"
#include "../requests/request.h"
#include "../responses/response.h"
#include <vector>
#include <string>

class PackingInteractor {
    private:
        std::vector<Box> boxes_;
        std::vector<Pallet> pallets_;
        std::vector<Crate> crates_;

        bool addBox(Box);
        bool packInBox(Art art);

    public:
        PackingInteractor();

        Response packAllArt(Request request);

        std::vector<Box> getBoxes();
        std::vector<Pallet> getPallets();
        std::vector<Crate> getCrates();
};

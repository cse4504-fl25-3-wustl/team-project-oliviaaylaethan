#include "packingInteractor.h"


PackingInteractor::PackingInteractor() {
    boxes_ = std::vector<Box>();
    pallets_ = std::vector<Pallet>();
    crates_ = std::vector<Crate>();
}

Response PackingInteractor::packAllArt(Request request) {
    Response response;
    std::cout << "all art packed" << std::endl;
    return response;
}

std::vector<Box> PackingInteractor::getBoxes() {
    return boxes_;
}

std::vector<Pallet> PackingInteractor::getPallets() {
    return pallets_;
}

std::vector<Crate> PackingInteractor::getCrates() {
    return crates_;
}

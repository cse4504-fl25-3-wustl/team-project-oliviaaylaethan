#include "interactors/packingInteractor.h"

int main() {
    PackingInteractor packingInteractor;
    Request request;
    Response response = packingInteractor.packAllArt(request);
    response.printWeightSummary();
    response.printPackingSummary();
    response.printBusinessIntelSummary();
    response.printEmailFormat();

    return 0;
}
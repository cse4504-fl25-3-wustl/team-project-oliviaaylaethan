#include "interactors/packingInteractor.h"
#include "parser/csvParser.h"

int main() {
    CsvParser parser;
    Request request = parser.parseFile("sample.csv");
    PackingInteractor packingInteractor;
    Response response = packingInteractor.packAllArt(request);
    response.printWeightSummary();
    response.printPackingSummary();
    response.printBusinessIntelSummary();
    response.printEmailFormat();

    return 0;
}
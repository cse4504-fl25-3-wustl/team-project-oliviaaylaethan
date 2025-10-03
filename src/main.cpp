#include "interactors/packingInteractor.h"
#include "parser/csvParser.h"
#include "responses/response.h"

int main() {
    CsvParser parser;
    Request request = parser.parseFile("/Users/ethanmoses/git/team-project-oliviaaylaethan/data_clean.csv");
    PackingInteractor packingInteractor;
    Response response = packingInteractor.packAllArt(request);
    response.printWeightSummary();
    response.printPackingSummary();
    response.printBusinessIntelSummary();
    response.printEmailFormat();

    return 0;
}
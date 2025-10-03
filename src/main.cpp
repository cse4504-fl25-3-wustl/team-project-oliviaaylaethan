#include "interactors/packingInteractor.h"
#include "parser/csvParser.h"
#include "responses/response.h"

int main() {
    CsvParser parser;
    Request request = parser.parseFile("C:/Users/aylab/cse454/team-project-oliviaaylaethan/Input1.csv");
    PackingInteractor packingInteractor;
    Response response = packingInteractor.packAllArt(request);
    response.printWeightSummary();
    response.printPackingSummary();
    response.printBusinessIntelSummary();
    response.printEmailFormat();

    return 0;
}
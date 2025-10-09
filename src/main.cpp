#include "interactors/packingInteractor.h"
#include "parser/csvParser.h"
#include "responses/response.h"

int main() {
    CsvParser parser;
    Request request = parser.parseFiles("/Users/oliviaguess/desktop/school/team-project-oliviaaylaethan/Input3.csv", "/Users/oliviaguess/desktop/school/team-project-oliviaaylaethan/Site_requirements.csv");
    PackingInteractor packingInteractor;
    Response response = packingInteractor.packAllArt(request);
    response.printWeightSummary();
    response.printPackingSummary();
    response.printBusinessIntelSummary();
    response.printEmailFormat();

    return 0;
}
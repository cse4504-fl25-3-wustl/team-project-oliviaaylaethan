#include "interactors/packingInteractor.h"
#include "parser/csvParser.h"
#include "responses/response.h"
#include <iostream>

int main() {
    std::string dataInputFile;
    std::string requirementsInputFile;
    bool run = true;
    int choice;

    std::cout << "\n---------- ART PACKER -----------" << std::endl;

    while (run) {
        std::cout << "\nWelcome to Art Packer." << std::endl;
        std::cout << "(1) Pack art\n(2) Quit" << std::endl;
        std::cout << "Please select an option: ";
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Invalid input. Please enter 1 or 2.\n";
            continue;
        }

        if (choice == 1) {
            std::cout << "Enter data input file: ";
            std::cin >> dataInputFile;
            std::cout << "\nEnter site requirements file: ";
            std::cin >> requirementsInputFile;
            std::cout << std::endl;

            CsvParser parser;
            Request request = parser.parseFiles(dataInputFile, requirementsInputFile);
            PackingInteractor packingInteractor;
            Response response = packingInteractor.packAllArt(request);
            response.printWeightSummary();
            response.printPackingSummary();
            response.printBusinessIntelSummary();
            response.printEmailFormat();
            
            continue;
        }
        else if (choice == 2) {
            run = false;
            break;
        }
        else {
            std::cout << "Please select 1 or 2." << std::endl;
            continue;
        }
    }

    return 0;
}
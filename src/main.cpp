#include <iostream>
#include <string>
#include "../include/hash.h"
#include "../include/analysis.h"

int main() {
    Hash hash;
    int userChoice;
    std::string inputStr, filename, fileContents;
    std::cout << "\nYou want to: \n"
                 "1. Hash a string\n"
                 "2. Hash a file\n"
                 "3  Compare execution time to sha256, md5, sha1, keccak\n"
                 "4  Test on determinism\n"
                 "5  Test on collisions\n"
                 "6. Test on Avalanche effect\n";

    std::cout << std::string(50, '-') << std::endl;
    std::cout << "Your choice: ";
    std::cin >> userChoice;
    validateInput(userChoice, 1, 7);

    switch (userChoice) {
        case 1:
            std::cout << "Enter a string: ";
            std::cin.ignore(1000, '\n');
            getline(std::cin, inputStr);
            std::cout << hash(inputStr) << std::endl;
            break;
        case 2:
            std::cout << "Enter a filename: ";
            std::cin >> filename;
            fileContents = readFileIntoStr(filename);
            std::cout << hash(fileContents) << std::endl;
            break;
        case 3:
            efficiencyTest();
            break;
        case 4:
            specificationTest();
            break;
        case 5:
            collisionTest();
            break;
        case 6:
            avalancheEffectComparison();
            break;
    }
    return 0;
}

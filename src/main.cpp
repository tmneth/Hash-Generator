#include <iostream>
#include <string>
#include "../include/hash.h"
#include "../include/analysis.h"

void showHelpMessage() {
    std::cerr
            << "Please use one of the following flags:\n"
            << "-h, --help: Show this help message\n"
            << "-f, --file: Hash a file\n"
            << "-s, --string: Hash an input string\n"
            << "-t, --tests: Perform tests\n"
            << "--salt: Add salt to hash\n"
            << std::endl;
}

int main(int argc, char *argv[]) {

    MYSHA mysha;
    std::string fileContents;

    if (argc == 1) {
        showHelpMessage();
        return 1;
    }

    for (int i = 0; i < argc; i++) {

        std::string arg = argv[i];

        if ((arg == "-h") || (arg == "--help") || argv[1][0] != '-') {
            showHelpMessage();
            return 1;
        } else if ((arg == "-s") || (arg == "--string")) {
            std::cout << mysha(argv[i + 1]) << std::endl;
        } else if ((arg == "-f") || (arg == "--file")) {
            fileContents = readFileIntoStr(argv[i + 1]);
            std::cout << mysha(fileContents) << std::endl;
        } else if ((arg == "-t") || (arg == "--tests")) {
            loadTests();
        } else if (arg == "--salt") {
            mysha.setSalt(argv[i + 1]);
        }

    }

    return 0;

}
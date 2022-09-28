#include <iostream>
#include <string>
#include "../include/hash.h"
#include "../include/analysis.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {

    MYSHA mysha;
    int userChoice;
    std::string inputStr, filename, fileContents;

    cout << "You want to: " << endl;
    cout << "1. Hash a string" << endl;
    cout << "2. Hash a file" << endl;
    cout << "3. I/O test" << endl;
    cout << "4. Measure time complexity" << endl;
    cout << "5. Test on collisions" << endl;
    cout << "6. Test on Avalanche effect" << endl;
    cout << "7. Compare hashing time to sha256, md5, sha1, keccak" << endl;

    cout << std::string(50, '-') << std::endl;
    cout << "Your choice: ";
    cin >> userChoice;
    validateInput(userChoice, 1, 7);

    switch (userChoice) {
        case 1:
            cout << "Enter a string: \n";
            std::cin.ignore(1000, '\n');
            getline(cin, inputStr);
            cout << mysha(inputStr) << endl;
            break;
        case 2:
            if (argc < 2) {
                std::cerr << "No valid input file was found." << endl;
            } else {
                fileContents = readFileIntoStr(argv[1]);
                cout << mysha(fileContents) << endl;
            }
            break;
        case 3:
            specificationTest();
            break;
        case 4:
            hashTimeTest();
            break;
        case 5:
            collisionTest();
            break;
        case 6:
            similarityTestComp();
            break;
        case 7:
            hashTimeComp();
            break;
    }
    return 0;
}

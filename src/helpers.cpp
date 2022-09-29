#include "../include/helpers.h"

std::string readFileIntoStr(const std::string &path) {
    std::ifstream input_file(path);
    if (!input_file.is_open()) {
        std::cerr << "Could not open the file - '"
                  << path << "'" << std::endl;
        exit(EXIT_FAILURE);
    }
    return std::string((std::istreambuf_iterator<char>(input_file)), std::istreambuf_iterator<char>());
}

void validateInput(int &userInput, int minValue, int maxValue) {
    while (std::cin.fail() || userInput < minValue || userInput > maxValue) {
        std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        std::cout << "Try again: ";
        std::cin >> userInput;
    }
}

std::string randomString(std::size_t length) {
    const std::string CHARACTERS = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    std::random_device random_device;
    std::mt19937 generator(random_device());
    std::uniform_int_distribution<> distribution(0, CHARACTERS.size() - 1);

    std::string randStr;

    for (std::size_t i = 0; i < length; ++i) {
        randStr += CHARACTERS[distribution(generator)];
    }

    return randStr;
}

void genRandPairs() {
    std::cout << "Generating text file..." << std::endl;

    std::string buffer;
    int length[4] = {10, 100, 500, 1000};

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 25000 / 2; j++)
            buffer.append(randomString(length[i]) + " " + randomString(length[i]) + "\n");

    std::ofstream fout("data/rand_comb.txt");
    fout << buffer.data();
    fout.close();
}

std::string mutateStr(std::string str) {
    str[0]++;
    return str;
}

void genSimPairs() {
    std::cout << "Generating text file..." << std::endl;

    std::string buffer;
    int length[4] = {10, 100, 500, 1000};

    for (int i = 0; i < 4; i++)
        for (int j = 0; j < 25000 / 2; j++) {
            std::string curStr = randomString(length[i]);
            buffer.append(curStr + " " + mutateStr(curStr) + "\n");
        }

    std::ofstream fout("data/sim_comb.txt");
    fout << buffer.data();
    fout.close();
}
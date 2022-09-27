#include "../include/analysis.h"

class Timer {

private:

    std::chrono::time_point<std::chrono::high_resolution_clock> start;

public:

    Timer() : start{std::chrono::high_resolution_clock::now()} {}

    void reset() { start = std::chrono::high_resolution_clock::now(); }

    double elapsed() const {
        return std::chrono::duration<double>(
                std::chrono::high_resolution_clock::now() - start)
                .count();
    }

};

/**
 * Test if hash function produces fixed size 256 bit hex string.
 * Test if hash procedure is deterministic.
 */
void specificationTest() {

    Hash hash;
    string filenames[7] = {"letter", "char", "rand_1000_1", "rand_1000_2", "sim_1500_1", "sim_1500_2", "empty"};
    vector<string> hex;
    bool isDeterministic = true;

    cout << "\nHashing contents of the files for the first time... " << endl;

    for (int i = 0; i < 7; ++i) {
        string fileContents = readFileIntoStr("data/" + filenames[i] + ".txt");
        string hashValue = hash.getHashVal(fileContents);
        hex.push_back(hashValue);
        cout << "File hashing: " << hashValue << "; length: " << hashValue.size() * 4 << " bit" << endl;
    }

    cout << "\nHashing contents of the files the for second time... " << endl;

    for (int i = 0; i < 7; ++i) {
        string fileContents = readFileIntoStr("data/" + filenames[i] + ".txt");
        string hashValue = hash.getHashVal(fileContents);
        isDeterministic = (hashValue == hex[i]) & isDeterministic;
        cout << "File hashing: " << hashValue << "; length: " << hashValue.size() * 4 << " bit" << endl;
    }

    cout << "\nHashing algorithm is" << (isDeterministic ? " " : " not ") << "deterministic." << endl;

}

/**
 * Measure execution time of the hashing function.
 */
void efficiencyTest() {

    Hash hash;
    Timer clock;

    int rounds{100};
    double duration{0};

    for (int i = 0; i < rounds; ++i) {

        std::ifstream fin("data/konstitucija.txt");
        string line;

        while (getline(fin, line)) {
            clock.reset();
            hash.getHashVal(line);
            duration += clock.elapsed();
        }
    }

    cout << "Average time: " << (duration / rounds) << endl;

}

/**
 * Count collisions.
 */
void collisionTest() {

    Hash hash;

    if (!std::filesystem::exists("data/rand_comb.txt"))
        genRandPairs();

    std::ifstream fin("data/rand_comb.txt");
    string line, str1, str2;

    int collisions = 0;
    for (int i = 0; i < 100000; ++i) {
        fin >> str1 >> str2;
        if (hash.getHashVal(str1) == hash.getHashVal(str2)) collisions++;
    }
    cout << (collisions ? "Collisions found: " + std::to_string(collisions) : "No collisions were found.") << endl;

    fin.close();

}

/**
 * Test Avalanche effect.
 */
void avalancheEffectTest() {

    Hash hash;
    if (!std::filesystem::exists("data/sim_comb.txt"))
        genSimPairs();

    std::ifstream fin("data/sim_comb.txt");
    string line, str1, str2;

    double hexMaxDiff = 0, hexMinDiff = 100.0, bitMaxDiff = 0, bitMinDiff = 100.0;
    double hexAvgDiff, bitAvgDiff, diffHex, diffBit;

    for (int j = 0; j < 100000; ++j) {
        int diff_hex = 0, diff_bit = 0;
        fin >> str1 >> str2;

        str1 = hash.getHashVal(str1);
        str2 = hash.getHashVal(str2);

        for (int i = 0; i < 64; i++) {

            if (str1[i] != str2[i]) diff_hex++;

            std::bitset<8> str1Byte(str1[i]);
            std::bitset<8> str2Byte(str2[i]);

            diff_bit += 8 - (str1Byte ^ str2Byte).count();
        }

        diffHex = (diff_hex / 64.0) * 100;
        diffBit = (diff_bit / 512.0) * 100;


        hexAvgDiff += diffHex;
        bitAvgDiff += diffBit;


        hexMaxDiff = std::max(hexMaxDiff, diffHex);
        bitMaxDiff = std::max(bitMaxDiff, diffBit);

        hexMinDiff = std::min(hexMinDiff, diffHex);
        bitMinDiff = std::min(bitMinDiff, diffBit);

    }

    fin.close();

    cout << std::fixed;

    cout << "Average difference (hex): " << std::setprecision(2) << hexAvgDiff / 100000 << "%" << endl;
    cout << "Average difference (bit): " << std::setprecision(2) << bitAvgDiff / 100000 << "%" << endl;

    cout << "Smallest difference (hex): " << std::setprecision(2) << hexMinDiff << "%" << endl;
    cout << "Smallest difference (bit): " << std::setprecision(2) << bitMinDiff << "%" << endl;

    cout << "Biggest difference (hex): " << std::setprecision(2) << hexMaxDiff << "%" << endl;
    cout << "Biggest difference (bit): " << std::setprecision(2) << bitMaxDiff << "%" << endl;

}

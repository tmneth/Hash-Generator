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

void specificationTest() {
    MYSHA mysha;
    string filenames[7] = {"letter", "char", "rand_1000_1", "rand_1000_2", "sim_1500_1", "sim_1500_2", "empty"};
    vector<string> hex;
    bool isDeterministic = true;

    for (auto &filename: filenames) {
        string fileContents = readFileIntoStr("data/" + filename + ".txt");
        string hashValue1 = mysha(fileContents);
        string hashValue2 = mysha(fileContents);

        isDeterministic = (hashValue1 == hashValue2) && isDeterministic;
        cout << "Hashing twice file: " << filename + ".txt; " << "Word size: " << hashValue1.size() << endl;
        cout << "Hash (1): " << hashValue1 << endl;
        cout << "Hash (2): " << hashValue2 << endl;
        cout << "Hashes are: " << (isDeterministic ? "identical" : "different") << endl;
        cout << endl;
    }

    cout << "Conclusion: hashing algorithm is" << (isDeterministic ? " " : " not ") << "deterministic." << endl;
}

template<class HashFunc>
void hashTimeTest() {
    HashFunc func;

    Timer clock;

    int rounds{100};
    vector<double> duration(10, 0);

    for (int m = 0; m < rounds; ++m) {
        string line;

        for (int i = 0, j = 1; i < 10; ++i, j *= 2) {

            std::ifstream fin("data/konstitucija.txt");

            for (int z = 0; z < j; z++) {
                getline(fin, line);
                clock.reset();
                func(line);
                duration[i] += clock.elapsed();
            }

            fin.close();

        }

    }
    for (int j = 1, i = 0; i < duration.size(); j *= 2, i++)
        cout << "Average hashing time to hash: " << j << " lines: " << std::to_string((duration[i] / rounds)) << "s."
             << endl;
}

void collisionTest() {
    MYSHA mysha;

    if (!std::filesystem::exists("data/rand_comb.txt"))
        genRandPairs();

    std::ifstream fin("data/rand_comb.txt");
    string line, str1, str2;

    int collisions = 0;
    for (int i = 0; i < 100000; ++i) {
        fin >> str1 >> str2;
        if (mysha(str1) == mysha(str2)) collisions++;
    }

    fin.close();

    cout << (collisions ? "Collisions found: " + std::to_string(collisions) : "No collisions were found.") << endl;
}

template<class HashFunc>
void similarityTest() {
    HashFunc func;


    std::ifstream fin("data/sim_comb.txt");
    string line, str1, str2;

    double hexMaxDiff = 0, hexMinDiff = 100.0, bitMaxDiff = 0, bitMinDiff = 100.0;
    double hexAvgDiff = 0, bitAvgDiff = 0, diffHex, diffBit;

    double wordSize = func("test").size();

    for (int j = 0; j < 100000; ++j) {
        int diffHexCount = 0, diffBitCount = 0;
        fin >> str1 >> str2;

        str1 = func(str1);
        str2 = func(str2);

        for (int i = 0; i < wordSize; i++) {

            if (str1[i] != str2[i]) diffHexCount++;

            std::bitset<8> str1Byte(str1[i]);
            std::bitset<8> str2Byte(str2[i]);

            diffBitCount += (str1Byte ^ str2Byte).count();

        }
        diffHex = (diffHexCount / wordSize) * 100;
        diffBit = (diffBitCount / (wordSize * 8)) * 100;

        hexAvgDiff += diffHex;
        bitAvgDiff += diffBit;

        hexMaxDiff = std::max(hexMaxDiff, diffHex);
        bitMaxDiff = std::max(bitMaxDiff, diffBit);

        hexMinDiff = std::min(hexMinDiff, diffHex);
        bitMinDiff = std::min(bitMinDiff, diffBit);
    }

    fin.close();

    cout << std::fixed;

    cout << "Avg difference (hex): " << std::setprecision(4) << hexAvgDiff / 100000 << "%" << endl;
    cout << "Avg difference (bit): " << std::setprecision(4) << bitAvgDiff / 100000 << "%" << endl;

    cout << "Min difference (hex): " << std::setprecision(4) << hexMinDiff << "%" << endl;
    cout << "Min difference (bit): " << std::setprecision(4) << bitMinDiff << "%" << endl;

    cout << "Max difference (hex): " << std::setprecision(4) << hexMaxDiff << "%" << endl;
    cout << "Max difference (bit): " << std::setprecision(4) << bitMaxDiff << "%" << endl;

    cout << endl;
}

void similarityTestComp() {
    if (!std::filesystem::exists("data/sim_comb.txt"))
        genSimPairs();

    cout << "MYSHA: " << endl;
    similarityTest<MYSHA>();
    cout << "SHA256: " << endl;
    similarityTest<SHA256>();
    cout << "MD5: " << endl;
    similarityTest<MD5>();
    cout << "SHA1: " << endl;
    similarityTest<SHA1>();
    cout << "KECCAK: " << endl;
    similarityTest<Keccak>();
}

void saltTest() {
    MYSHA mysha;
    string filenames[7] = {"letter", "char", "rand_1000_1", "rand_1000_2", "sim_1500_1", "sim_1500_2", "empty"};
    vector<string> hex;
    bool isDeterministic = true;

    for (auto &filename: filenames) {
        string fileContents = readFileIntoStr("data/" + filename + ".txt");

        string hashValue1 = mysha(fileContents);
        mysha.setSalt(randomString(10));
        string hashValue2 = mysha(fileContents);

        cout << "Hashing twice file: " << filename + ".txt; " << "Word size: " << hashValue1.size() << endl;
        cout << "Hash (original): " << hashValue1 << endl;
        cout << "Hash (salted): " << hashValue2 << endl;
        cout << endl;
    }

}


void hashTimeComp() {
    cout << "MYSHA: " << endl;
    hashTimeTest<MYSHA>();
    cout << "SHA256: " << endl;
    hashTimeTest<SHA256>();
    cout << "MD5: " << endl;
    hashTimeTest<MD5>();
    cout << "SHA1: " << endl;
    hashTimeTest<SHA1>();
    cout << "KECCAK: " << endl;
    hashTimeTest<Keccak>();
}

void loadTests() {
    int userChoice;
    string inputStr, filename, fileContents;

    cout << "Choose a test:\n"
         << "1. I/O test\n"
         << "2. Time complexity test\n"
         << "3. Collision  resistance test\n"
         << "4. Avalanche effect test\n"
         << "5. Comparison with sha256, md5, sha1, keccak\n"
         << "6. Salt test\n";

    cout << string(50, '-') << endl;
    cout << "Your choice: ";
    cin >> userChoice;
    validateInput(userChoice, 1, 6);

    switch (userChoice) {
        case 1:
            specificationTest();
            break;
        case 2:
            hashTimeTest<MYSHA>();
            break;
        case 3:
            collisionTest();
            break;
        case 4:
            similarityTestComp();
            break;
        case 5:
            hashTimeComp();
            break;
        case 6:
            saltTest();
            break;
    }
}

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
        cout << "File: " << filename + ".txt; " << "Word size: " << hashValue1.size() << endl;
        cout << "Hash: " << hashValue1 << endl;
        cout << endl;
    }

    cout << "Hashing algorithm is" << (isDeterministic ? " " : " not ") << "deterministic." << endl;
}

void hashTimeTest() {
    MYSHA mysha;

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
                mysha(line);
                duration[i] += clock.elapsed();
            }
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
void similarityTest(int wordSize) {
    HashFunc object;

    if (!std::filesystem::exists("data/sim_comb.txt"))
        genSimPairs();

    std::ifstream fin("data/sim_comb.txt");
    string line, str1, str2;

    double hexMaxDiff = 0, hexMinDiff = 100.0, bitMaxDiff = 0, bitMinDiff = 100.0;
    double hexAvgDiff = 0, bitAvgDiff = 0, diffHex, diffBit;

    for (int j = 0; j < 100000; ++j) {
        int diff_hex = 0, diff_bit = 0;
        fin >> str1 >> str2;

        str1 = object(str1);
        str2 = object(str2);

        for (int i = 0; i < wordSize; i++) {

            if (str1[i] != str2[i]) diff_hex++;

            std::bitset<8> str1Byte(str1[i]);
            std::bitset<8> str2Byte(str2[i]);

            diff_bit += 8 - (str1Byte ^ str2Byte).count();
        }

        diffHex = (diff_hex / (double) wordSize) * 100;
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

    cout << endl;
}

void similarityTestComp() {
    cout << "MYSHA: " << endl;
    similarityTest<MYSHA>(64);
    cout << "SHA256: " << endl;
    similarityTest<SHA256>(64);
    cout << "MD5: " << endl;
    similarityTest<MD5>(32);
    cout << "SHA1: " << endl;
    similarityTest<SHA1>(40);
    cout << "KECCAK: " << endl;
    similarityTest<Keccak>(64);
}


struct HashFunc {
    string name;
    double hashingTime;
};

void hashTimeComp() {
    MYSHA mysha;
    SHA256 sha256;
    MD5 md5;
    SHA1 sha1;
    Keccak keccak;

    Timer clock;

    int rounds{100};

    HashFunc func[5] = {{"MYSHA"},
                        {"SHA256"},
                        {"MD5"},
                        {"SHA1"},
                        {"KECCAK"}};

    for (int i = 0; i < rounds; ++i) {

        std::ifstream fin("data/konstitucija.txt");
        string line;

        while (getline(fin, line)) {

            clock.reset();
            mysha(line);
            func[0].hashingTime += clock.elapsed();

            clock.reset();
            sha256(line);
            func[1].hashingTime += clock.elapsed();

            clock.reset();
            md5(line);
            func[2].hashingTime += clock.elapsed();

            clock.reset();
            sha1(line);
            func[3].hashingTime += clock.elapsed();

            clock.reset();
            keccak(line);
            func[4].hashingTime += clock.elapsed();
        }

        fin.close();

    }

    for (const HashFunc &h: func)
        cout << h.name << " average hashing time: " << (h.hashingTime / rounds) << "s." << endl;
}
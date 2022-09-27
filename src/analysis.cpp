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
    Hash hash;
    string filenames[7] = {"letter", "char", "rand_1000_1", "rand_1000_2", "sim_1500_1", "sim_1500_2", "empty"};
    vector<string> hex;
    bool isDeterministic = true;

    cout << "\nHashing contents of the files for the first time... " << endl;

    for (int i = 0; i < 7; ++i) {
        string fileContents = readFileIntoStr("data/" + filenames[i] + ".txt");
        string hashValue = hash(fileContents);
        hex.push_back(hashValue);
        cout << "File hashing: " << hashValue << "; length: " << hashValue.size() * 4 << " bit" << endl;
    }

    cout << "\nHashing contents of the files the for second time... " << endl;

    for (int i = 0; i < 7; ++i) {
        string fileContents = readFileIntoStr("data/" + filenames[i] + ".txt");
        string hashValue = hash(fileContents);
        isDeterministic = (hashValue == hex[i]) & isDeterministic;
        cout << "File hashing: " << hashValue << "; length: " << hashValue.size() * 4 << " bit" << endl;
    }

    cout << "\nHashing algorithm is" << (isDeterministic ? " " : " not ") << "deterministic." << endl;
}

struct HashFunc {
    string name;
    double hashingTime;
};

void efficiencyTest() {
    Hash hash;
    SHA256 sha256;
    MD5 md5;
    SHA1 sha1;
    Keccak keccak;

    Timer clock;

    int rounds{100};

    HashFunc func[5] = {{"hash"},
                        {"sha256"},
                        {"md5"},
                        {"sha1"},
                        {"keccak"}};

    for (int i = 0; i < rounds; ++i) {

        std::ifstream fin("data/konstitucija.txt");
        string line;

        while (getline(fin, line)) {

            clock.reset();
            hash(line);
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
    }
    for (const HashFunc &h: func)
        cout << h.name << " average hashing time: " << (h.hashingTime / rounds) << "ms." << endl;
}

void collisionTest() {
    Hash hash;

    if (!std::filesystem::exists("data/rand_comb.txt"))
        genRandPairs();

    std::ifstream fin("data/rand_comb.txt");
    string line, str1, str2;

    int collisions = 0;
    for (int i = 0; i < 100000; ++i) {
        fin >> str1 >> str2;
        if (hash(str1) == hash(str2)) collisions++;
    }
    cout << (collisions ? "Collisions found: " + std::to_string(collisions) : "No collisions were found.") << endl;

    fin.close();
}

template<class HashFunc>
void avalancheEffectTest(double blockSize) {
    HashFunc object;

    if (!std::filesystem::exists("data/sim_comb.txt"))
        genSimPairs();

    std::ifstream fin("data/sim_comb.txt");
    string line, str1, str2;

    double hexMaxDiff = 0, hexMinDiff = 100.0, bitMaxDiff = 0, bitMinDiff = 100.0;
    double hexAvgDiff = 0, bitAvgDiff = 0, diffHex, diffBit;
    double wordSize = object("word").size();

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

        diffHex = (diff_hex / wordSize) * 100;
        diffBit = (diff_bit / blockSize) * 100;


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

void avalancheEffectComparison() {
    Hash hash;
    SHA256 sha256;
    MD5 md5;
    SHA1 sha1;
    CRC32 crc32;
    Keccak keccak;

    cout << "HASH: " << endl;
    avalancheEffectTest<Hash>(512);
    cout << "SHA256: " << endl;
    avalancheEffectTest<SHA256>(512);
    cout << "MD5: " << endl;
    avalancheEffectTest<MD5>(512);
    cout << "SHA1: " << endl;
    avalancheEffectTest<SHA1>(512);
    cout << "KECCAK: " << endl;
    avalancheEffectTest<Keccak>(512);
}
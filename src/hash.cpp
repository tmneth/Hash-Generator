#include "../include/hash.h"

/**
 * Initialize 8 8-bit semi-random keys.
 */
void Hash::initKeys() {

    for (int i = 0; i < 8; ++i) {
        std::bitset<8> b(std::rand());
        m_keys[i] = b.to_ulong();
    }

}

/**
 * Convert an input string to a vector consisting of one 8 bit value per ASCII character.
 */
std::vector<uint8_t> Hash::strToAscii(const std::string &input) {

    std::vector<uint8_t> bytes;

    for (char c: input) {

        std::bitset<8> b(c);
        bytes.push_back(b.to_ulong());

    }

    return bytes;
}


/**
 * Calculate & add the required padding to the message.
 */
std::vector<uint8_t> Hash::padBin(std::vector<uint8_t> bytes) {

    int k = 0;
    uint64_t l = bytes.size() * 8;

    while ((l + k) % 512 != 448) ++k;

    for (int i = 0; i < k / 8; i++)
        bytes.push_back(0x80);

    for (const uint8_t key: m_keys)
        bytes.push_back(key);

    return bytes;
}


/**
 * Reduce to 8 32-bit words & compute hash values.
 */
void Hash::computeHash(const std::string &input, std::vector<uint8_t> bytes) {

    std::vector<std::uint32_t> words(16, 0);

    for (int i = 0, m = 0; i < bytes.size() / 64; ++i, m += 64)
        for (int j = m, z = 0; j < m + 64; j += 4, z++) {
            uint32_t ui32 =
                    (uint32_t(bytes[j]) << 24) | (uint32_t(bytes[j + 1]) << 16) |
                    (uint32_t(bytes[j + 2]) << 8) |
                    uint32_t(bytes[j + 3]);
            words[z] ^= ui32;
        }

    bytes.clear();


    for (int i = 0; i < words.size(); i++)
        for (int j = 0; j < words.size(); j++)
            words[i] += (words[j] >> 2);


    for (int i = 0, j = words.size() - 1; i < words.size() / 2; i++, j--)
        m_hashVal[i] = (words[i] + words[j]) << 2;

    for (int i = 4; i < 8; ++i)
        m_hashVal[i] += m_hashVal[i - 4] + (m_hashVal[i - 3] | m_hashVal[i - 2]) + m_hashVal[i - 1];


    for (int j = 0; j < 4; ++j)
        m_hashVal[j] += (m_hashVal[j + 1] ^ (m_hashVal[j + 3]) >> (input.size() % 2));

}

/**
 * Convert generated hash values to a hexadecimal string.
 */
std::string Hash::hashToHex() {

    std::stringstream stream;
    for (int i = 0; i < 8; ++i)
        stream << std::hex << std::setw(8) << std::setfill('0') << m_hashVal[i];

    return stream.str();

}

/**
 * Calculate and return final hash value.
 */
std::string Hash::getHashVal(const std::string &input) {

    std::srand(input.size());
    initKeys();

    std::vector<uint8_t> bytes;
    bytes = strToAscii(input);
    bytes = padBin(bytes);
    computeHash(input, bytes);

    return hashToHex();

}
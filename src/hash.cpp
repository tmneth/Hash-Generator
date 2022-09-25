#include "../include/hash.h"

/**
 * Convert an input string to a vector consisting of one 8 bit value per ASCII character.
 */
std::vector<uint8_t> Hash::str_to_ascii(std::string input) {

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
std::vector<uint8_t> Hash::pad_bin(std::vector<uint8_t> bytes) {

    int k = 0;
    uint64_t l = bytes.size() * 8;

    while ((l + k) % 512 != 448) ++k;

    for (int i = 0; i < k / 8; i++)
        bytes.push_back(0x80);

    for (int i = 1; i < 9; ++i)
        bytes.push_back(l >> (64 - i * 8));


//    for (int i = 0; i < bytes.size(); i = i + 4) {
//        std::cout << i + 1 << ": " << (int) bytes[i] << "     "
//                  << i + 2 << ": " << (int) bytes[i + 1] << "     "
//                  << i + 3 << ": " << (int) bytes[i + 2] << "     "
//                  << i + 4 << ": " << (int) bytes[i + 3] << std::endl;
//    }


    return bytes;
}


/**
 * Compute hash values.
 */
void Hash::compute_hash(std::string input, std::vector<uint8_t> bytes) {

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
            words[i] += (words[j] / 32) + (input.size() % 2);


    for (int i = 0, j = words.size() - 1; i < words.size() / 2; i++, j--)
        m_hash_values[i] = words[i] + words[j];

    for (int i = 4; i < 8; ++i)
        m_hash_values[i] +=
                (m_hash_values[i - 4] >> 2) + (m_hash_values[i - 3] | m_hash_values[i - 2]) + m_hash_values[i - 1];

    for (int j = 0; j < 4; ++j) {
        m_hash_values[j] += (m_hash_values[j + 1] ^ m_hash_values[j + 3]) >> (input.size() % 2);
    }

    for (int i = 0; i < 8; i++)
        m_hash_values[i] = m_hash_values[i] & 0xFFFFFFFF;


}

/**
 * Convert generated hash values to a hexadecimal string.
 */
std::string Hash::hash_to_hex() {

    std::stringstream stream;
    for (int i = 0; i < 8; ++i)
        stream << std::hex << std::setw(8) << std::setfill('0') << (m_const_values[i] ^ m_hash_values[i]);

    return stream.str();
}

/**
 * Calculate and return final hash value.
 */
std::string Hash::get_hash_val(std::string input) {

    std::vector<uint8_t> bytes;
    bytes = str_to_ascii(input);
    bytes = pad_bin(bytes);
    compute_hash(input, bytes);

    return hash_to_hex();
}

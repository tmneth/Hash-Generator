#pragma once

#include <iostream>
#include <bitset>
#include <vector>
#include <string>
#include <iomanip>

class Hash {

private:

    std::uint32_t m_hash_values[8];

    std::uint32_t m_const_values[8] = {
            0x42a2f98, 0x71374491, 0xb5c0fbcf, 0xe9b5dba5, 0x3956c25b, 0x59f111f1,
            0x923f82a4, 0xab1c5ed5
    };

    std::vector<uint8_t> str_to_ascii(std::string input);

    std::vector<uint8_t> pad_bin(std::vector<uint8_t> bytes);

    void compute_hash(std::string input, std::vector<uint8_t> bytes);

    std::string hash_to_hex();

public:

    Hash() = default;

    std::string get_hash_val(std::string input);

    ~Hash() = default;

};

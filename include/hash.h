#pragma once

#include <iostream>
#include <bitset>
#include <vector>
#include <string>
#include <iomanip>

class MYSHA {

private:

    std::uint32_t m_hashVal[8];

    std::uint8_t m_keys[8];

    void initKeys();

    std::vector<uint8_t> addPad(std::vector<uint8_t> bytes);

    void computeHash(const std::string &input, std::vector<uint8_t> bytes);

    std::string hashToHex();

public:

    MYSHA() = default;

    std::string getHashVal(const std::string &input);

    std::string operator()(const std::string &text);

};
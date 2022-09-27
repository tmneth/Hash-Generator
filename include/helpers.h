#pragma once

#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include <random>

std::string readFileIntoStr(const std::string &path);

void validateInput(int &user_input, int min_value, int max_value);

std::string randomString(std::size_t length);

void genRandPairs();

void genSimPairs();
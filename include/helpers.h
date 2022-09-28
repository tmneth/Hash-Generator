#pragma once

#include <iostream>
#include <chrono>
#include <fstream>
#include <string>
#include <random>

std::string readFileIntoStr(const std::string &path);

void validateInput(int &userInput, int minValue, int maxValue);

std::string randomString(std::size_t length);

void genRandPairs();

void genSimPairs();
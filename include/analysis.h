#pragma once

#include <iostream>
#include <vector>
#include <iomanip>
#include <chrono>
#include <filesystem>
#include "../include/hash.h"
#include "../include/helpers.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

void specificationTest();

void efficiencyTest();

void collisionTest();

void avalancheEffectTest();
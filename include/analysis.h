#pragma once

#include <iostream>
#include <vector>
#include <iomanip>
#include <chrono>
#include <filesystem>
#include "../include/hash.h"
#include "../include/helpers.h"

#include "../lib/hash-library/sha256.h"
#include "../lib/hash-library/md5.h"
#include "../lib/hash-library/sha1.h"
#include "../lib/hash-library/keccak.h"

using std::cout;
using std::cin;
using std::endl;
using std::string;
using std::vector;

void specificationTest();

void hashTimeTest();

void collisionTest();

void similarityTestComp();

void hashTimeComp();
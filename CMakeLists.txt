cmake_minimum_required(VERSION 3.23)
project(hash_generator)

set(CMAKE_CXX_STANDARD 23)

add_executable(hash_generator src/analysis.cpp src/main.cpp src/hash.cpp src/helpers.cpp include/hash.h include/analysis.h include/helpers.h)

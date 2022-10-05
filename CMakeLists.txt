cmake_minimum_required(VERSION 3.23)
project(hash_generator)

set(CMAKE_CXX_STANDARD 23)

include_directories(include)
include_directories(lib)

file(GLOB SOURCES CONFIGURE_DEPENDS "src/*.cpp" "lib/hash-library/sha256.cpp"  "lib/hash-library/md5.cpp" "lib/hash-library/sha1.cpp" "lib/hash-library/keccak.cpp")
add_executable(test ${SOURCES})
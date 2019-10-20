cmake_minimum_required(VERSION 3.10)
project(emulator8080 VERSION 0.0.1 LANGUAGES CXX)

find_package(cxxopts REQUIRED)

add_executable(disassemble src/disassemble.cpp)
target_compile_features(disassemble PRIVATE cxx_std_17)
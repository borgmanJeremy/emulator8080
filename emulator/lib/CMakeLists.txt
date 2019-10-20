cmake_minimum_required(VERSION 3.10)
project(emulator8080 VERSION 0.0.1 LANGUAGES CXX)

find_package(cxxopts REQUIRED)
include(CodeCoverage)

add_library(cpu_8080) 

target_sources(cpu_8080 
    PRIVATE 
        "src/cpu_8080_math_add.cpp" 
        "src/cpu_8080_math_sub.cpp" 
        "src/cpu_8080_mov.cpp"
        "src/cpu_8080_logical.cpp"
        "src/cpu_8080_branch.cpp"
        "src/cpu_8080_reg_misc.cpp"
        "src/cpu_8080_reserved.cpp"
        "src/cpu_8080.cpp"
        )
target_include_directories(cpu_8080 PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
)
target_add_code_coverage(cpu_8080 ../bin/test_8080)
target_compile_options(cpu_8080 PRIVATE -Wall -Wextra -Wshadow -Wpedantic)
target_compile_features(cpu_8080 PRIVATE cxx_std_17)


add_library(cpu_8080::cpu_8080 ALIAS cpu_8080) 
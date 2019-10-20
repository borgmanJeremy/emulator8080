cmake_minimum_required(VERSION 3.10)
project(emulator8080 VERSION 0.0.1 LANGUAGES CXX)

find_package(cxxopts REQUIRED)

add_executable(disassemble) 

target_sources(disassemble 
    PRIVATE 
        "src/disassemble.cpp" 
        )
target_include_directories(disassemble PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/include/
)
target_link_libraries(disassemble
        PRIVATE 
            cpu_8080::cpu_8080)

target_compile_options(disassemble PRIVATE -Wall -Wextra -Wshadow -Wpedantic )
target_compile_features(disassemble PRIVATE cxx_std_17)


add_executable(cpudiag) 

target_sources(cpudiag 
    PRIVATE 
        "src/cpudiag.cpp" 
        )
target_include_directories(cpudiag PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/include/
)
target_link_libraries(cpudiag
        PRIVATE 
            cpu_8080::cpu_8080)

target_compile_options(cpudiag PRIVATE -Wall -Wextra -Wshadow -Wpedantic )
target_compile_features(cpudiag PRIVATE cxx_std_17)
cmake_minimum_required(VERSION 3.10)
project(emulator8080 VERSION 0.0.1 LANGUAGES CXX)

find_package(cxxopts REQUIRED)
find_package(SDL2 REQUIRED)

add_executable(disassemble) 

target_sources(disassemble 
    PRIVATE 
        "src/disassemble.cpp" 
        )
target_include_directories(disassemble PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/include/
    ${cxxopts_INCLUDE_DIRS}
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
    ${cxxopts_INCLUDE_DIRS}
)
target_link_libraries(cpudiag
        PRIVATE 
            cpu_8080::cpu_8080)

target_compile_options(cpudiag PRIVATE -Wall -Wextra -Wshadow -Wpedantic )
target_compile_features(cpudiag PRIVATE cxx_std_17)


add_executable(space_invaders) 

target_sources(space_invaders 
    PRIVATE 
        "src/space_invaders_main.cpp"
        "src/space_invaders.cpp" 
        )
target_include_directories(space_invaders PRIVATE 
    ${SDL2_INCLUDE_DIRS} 
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/include/
    ${CMAKE_CURRENT_SOURCE_DIR}/include/
)

#TODO, understand what conanlibs is actually doing
target_link_libraries(space_invaders
        PRIVATE 
            ${CONAN_LIBS} 
            cpu_8080::cpu_8080)

target_compile_options(space_invaders PRIVATE -Wall -Wextra -Wshadow -Wpedantic )
target_compile_features(space_invaders PRIVATE cxx_std_17)
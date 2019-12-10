cmake_minimum_required(VERSION 3.10)
project(emulator8080 VERSION 0.0.1 LANGUAGES CXX)

find_package(Catch2 REQUIRED)
include(CTest)
include(Catch)
include(ParseAndAddCatchTests)

add_executable(test_8080)

target_sources(test_8080 
            PRIVATE
                "test_main.cpp"
                "test_class.cpp"
                "test_smart_counter.cpp"
                 "test_8080_add.cpp"
                 "test_8080_sub.cpp"
                 "test_8080_flags.cpp"
                 "test_8080_memory.cpp"
                 "test_8080_logical.cpp"
                 "test_8080_branch.cpp"
                 "test_8080_register_mov.cpp"
                 "test_8080_mem_mov.cpp"
                 "test_8080_reg_misc.cpp"
                 "test_memory.cpp")
target_compile_features(test_8080 PRIVATE cxx_std_17)
target_include_directories(test_8080
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/../lib/include/
    )

target_link_libraries(test_8080
        PRIVATE 
            Catch2::Catch2
            cpu_8080::cpu_8080)

ParseAndAddCatchTests(test_8080)
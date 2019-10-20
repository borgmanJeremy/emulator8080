cmake_minimum_required(VERSION 3.10)
project(emulator8080 VERSION 0.0.1 LANGUAGES CXX)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CONAN_SYSTEM_INCLUDES ON)
option(BUILD_CODE_COVERAGE "Builds Code Coverage Report" ON)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_subdirectory(lib)
add_subdirectory(test)
add_subdirectory(app)
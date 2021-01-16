cmake_minimum_required(VERSION 3.8)
project(Chippy
DESCRIPTION "Chip8 Emulator in C++"
HOMEPAGE_URL https://github.com/jarviliam/chippy
LANGUAGES CXX)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
find_package(SDL2 REQUIRED)

include_directories(${SDL2_INCLUDE_DIRS})
add_subdirectory(src)


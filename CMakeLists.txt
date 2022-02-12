cmake_minimum_required(VERSION 3.8)
project(
  Chippy
  DESCRIPTION "Chip8 Emulator in C++"
  HOMEPAGE_URL https://github.com/jarviliam/chippy
  LANGUAGES CXX)

# if(NOT CMAKE_BUILD_TYPE) set(CMAKE_BUILD_TYPE "Release" CACHE STRING "Choose
# the type of build, options are: Debug Release." FORCE) endif(NOT
# CMAKE_BUILD_TYPE)

set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED YES)

add_compile_options(-Wall -Wextra -Wpedantic)

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/609281088cfefc76f9d0ce82e1ff6c30cc3591e5.zip
)
FetchContent_MakeAvailable(googletest)

include_directories("${PROJECT_SOURCE_DIR}/include")
# Add Sources
file(GLOB SOURCES "${PROJECT_SOURCE_DIR}/src/*.cpp")

add_library(ChipLib ${SOURCES})

find_package(SDL2 REQUIRED)
include_directories(Chippy ${SDL2_INCLUDE_DIRS})

add_executable(Chippy "src/main.cpp")
target_link_libraries(Chippy ChipLib ${SDL2_LIBRARIES})

add_subdirectory(tests)

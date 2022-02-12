cmake_minimum_required(VERSION 3.8)

set(BINARY ChippyTests)

enable_testing()

file(
  GLOB_RECURSE TEST_SOURCES
  LIST_DIRECTORIES false
  *.hpp *.cpp)

# set(SOURCES ${TEST_SOURCES})

include_directories(${BINARY} "${PROJECT_SOURCE_DIR}/src")
add_executable(${BINARY} "opcode_test.cpp")

add_test(NAME ${BINARY} COMMAND ${BINARY})

target_link_libraries(${BINARY} PRIVATE ChipLib gtest gtest_main
                                        ${SDL2_LIBRARIES})

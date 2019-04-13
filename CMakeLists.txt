set(KNAPSAK knapsak)
project(${KNAPSAK})
cmake_minimum_required(VERSION 3.11)

include_directories(${CMAKE_CURRENT_SOURCE_DIR}/include)
file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*.cc)

add_executable(${KNAPSAK} ${SOURCE_FILES})		
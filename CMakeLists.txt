cmake_minimum_required(VERSION 2.8)
project(GCJIT)

add_subdirectory(libs)

# C++11
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11") 

include_directories(${GCJIT_SOURCE_DIR} ${GCJIT_SOURCE_DIR}/libs/asmjit/src)

add_executable(gcjit main.cpp)

target_link_libraries(gcjit asmjit)
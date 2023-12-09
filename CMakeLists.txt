# Minimum version required
cmake_minimum_required(VERSION 3.20)

# Project name and language
project(DecodeNcodeAnything CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable all compiler warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -O3 -O2")

# Add subdirectories
add_subdirectory(app)
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(modules)

# Import the ReedSolomon module
#target_link_libraries(encode PRIVATE ReedSolomon::ReedSolomon)
#target_link_libraries(decode PRIVATE ReedSolomon::ReedSolomon)


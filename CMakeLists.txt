# Minimum version required
cmake_minimum_required(VERSION 3.20)

# Project name and language
project(DecodeNcodeAnything CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable all compiler warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Source files
set(SOURCES
    src/main.cpp
    src/other_file.cpp
)

# Add executable


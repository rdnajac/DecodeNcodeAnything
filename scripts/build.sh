#!/bin/bash

# Create a build directory if it doesn't exist
mkdir -vp build

# Change to the build directory
cd build || exit

# Run CMake to configure the project with verbose output
cmake ..

# Build the project
make

# Optionally, install the project
#make install

# Return to the original directory
cd ..


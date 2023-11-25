# Minimum version required
cmake_minimum_required(VERSION 3.20)

# Project name and language
project(DecodeNcodeAnything CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable all compiler warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# Add subdirectories
add_subdirectory(src)
add_subdirectory(tests)
add_subdirectory(encode)

# ===================================
# Installation Configuration
# ===================================

# Specify the installation directory for executables
#install(TARGETS test_oligo test_utils test_io DESTINATION bin/tests)

# Create separate directories for libraries and binaries
#set_target_properties(mylib PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
#set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
#set_target_properties(test_oligo test_utils test_io PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests")


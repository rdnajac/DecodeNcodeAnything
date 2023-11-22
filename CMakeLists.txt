# Minimum version required
cmake_minimum_required(VERSION 3.20)

# Project name and language
project(DecodeNcodeAnything CXX)

# Set C++ standard
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Enable all compiler warnings
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

# ===================================
# Library Configuration
# ===================================

# Source files for the library
set(LIB_SOURCES
    src/oligo.cpp
    src/utils.cpp
)

# Add library
add_library(mylib ${LIB_SOURCES})
target_include_directories(mylib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

# ===================================
# Main Executable Configuration
# ===================================

# Source files for the main executable
set(MAIN_SOURCES
    src/main.cpp
)

# Add main executable
add_executable(main ${MAIN_SOURCES})
target_link_libraries(main PRIVATE mylib)
target_include_directories(main PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# ===================================
# Test Executables Configuration
# ===================================

# Source files for the test executables
set(TEST_OLIGO_SOURCES
    tests/test_oligo.cpp
)

set(TEST_UTILS_SOURCES
    tests/test_utils.cpp
)

# Add test executables
add_executable(test_oligo ${TEST_OLIGO_SOURCES})
add_executable(test_utils ${TEST_UTILS_SOURCES})
target_link_libraries(test_oligo PRIVATE mylib)
target_link_libraries(test_utils PRIVATE mylib)
target_include_directories(test_oligo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)
target_include_directories(test_utils PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)

# ===================================
# Installation Configuration
# ===================================

# Specify the installation directory for executables
install(TARGETS main DESTINATION bin)
install(TARGETS test_oligo test_utils DESTINATION bin/tests)

# Create separate directories for libraries and binaries
set_target_properties(mylib PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set_target_properties(main PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")
set_target_properties(test_oligo test_utils PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/tests")


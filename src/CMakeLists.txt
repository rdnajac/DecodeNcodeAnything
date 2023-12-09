# src/CMakeLists.txt

# List of source files in the src directory
set(SRC_FILES
    codec.cpp
    io.cpp
    oligo.cpp
    utils.cpp
)

# Create a library from the source files
add_library(my_library ${SRC_FILES})

# Add include directories
target_include_directories(my_library PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Conditionally add ReedSolomon module for oligo.cpp
target_sources(my_library PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/oligo.cpp)

# Define a compile-time flag for oligo.cpp
#target_compile_definitions(my_library PRIVATE OLIGO_NEEDS_REEDSOLOMON)


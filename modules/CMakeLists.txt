# extern/CMakeLists.txt

# Module source files
set(MODULE_SOURCES
    myRSmodule.cpp
)

# Module header file
set(MODULE_HEADERS
    primes.hpp
)

# Create the module library
add_library(ReedSolomon MODULE ${MODULE_SOURCES} ${MODULE_HEADERS})

# Specify include directories for the module
target_include_directories(ReedSolomon PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})  # Corrected line

# Set the language for the module target
set_target_properties(ReedSolomon PROPERTIES LINKER_LANGUAGE CXX)

# Export the module for use in the parent project
install(TARGETS ReedSolomon LIBRARY DESTINATION lib/cmake/ReedSolomon)


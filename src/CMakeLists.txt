# CMakeLists.txt in the src directory

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

# Uncomment this if you want to create an executable
# add_executable(main main.cpp)
# target_link_libraries(main PRIVATE mylib)
#

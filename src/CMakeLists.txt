# CMakeLists.txt in the src directory

# Add all source files in the src directory
add_library(mylib oligo.cpp utils.cpp)

# Include the include directory for header files
target_include_directories(mylib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# Optionally install the library
# install(TARGETS mylib DESTINATION lib)

# Uncomment this if you want to create an executable
# add_executable(main main.cpp)
# target_link_libraries(main PRIVATE mylib)

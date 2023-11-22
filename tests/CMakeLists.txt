# CMakeLists.txt in the tests directory

# Add the test executables
add_executable(test_oligo test_oligo.cpp)
add_executable(test_utils test_utils.cpp)

# Link against the library
target_link_libraries(test_oligo PRIVATE mylib)
target_link_libraries(test_utils PRIVATE mylib)

# Include the include directory for header files
target_include_directories(test_oligo PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
target_include_directories(test_utils PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)


# tests/CMakeLists.txt

# List of test source files in the tests directory
set(TEST_FILES
    test_io.cpp
    test_oligo.cpp
    test_utils.cpp
    simulate_encoded_fastq.cpp
)

# Create a test executable for each test file
foreach(TEST_FILE ${TEST_FILES})
    get_filename_component(TEST_NAME ${TEST_FILE} NAME_WE)
    add_executable(${TEST_NAME} ${TEST_FILE})
    target_include_directories(${TEST_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/../include)
endforeach()

target_link_libraries(test_io PRIVATE my_library)
target_link_libraries(test_oligo PRIVATE my_library)
target_link_libraries(test_utils PRIVATE my_library)
target_link_libraries(simulate_encoded_fastq PRIVATE my_library)


# encode/CMakeLists.txt

add_executable(encode encode.cpp)
add_executable(decode decode.cpp)

target_link_libraries(encode my_library)
target_link_libraries(decode my_library)


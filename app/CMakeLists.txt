# app/CMakeLists.txt

add_executable(encode encode.cpp)
target_link_libraries(encode PRIVATE my_library)
#target_link_libraries(encode PRIVATE ReedSolomon)

add_executable(decode decode.cpp)
target_link_libraries(decode PRIVATE my_library)
#target_link_libraries(decode PRIVATE ReedSolomon)


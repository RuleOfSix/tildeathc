cmake_minimum_required(VERSION 3.22)

add_executable(tokenize_test tokenize_test.c)
target_include_directories(tokenize_test PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
										 PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
										 )
target_link_libraries(tokenize_test PRIVATE tokenize)
add_test(NAME TokenizeTest COMMAND tokenize_test)

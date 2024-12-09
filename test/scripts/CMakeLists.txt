cmake_minimum_required(VERSION 3.22)

add_executable(generate_codegen_test_input generate_codegen_test_input.c)
target_include_directories(generate_codegen_test_input PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
													   PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
													   )
target_link_libraries(generate_codegen_test_input PRIVATE tokenize parse il validate_il)

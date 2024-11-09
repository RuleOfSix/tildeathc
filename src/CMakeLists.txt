cmake_minimum_required(VERSION 3.22)

add_library(util STATIC util.c)
target_include_directories(util PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
								PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
								)
add_library(tokenize STATIC tokenize.c)
target_include_directories(tokenize PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc" 
									PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
									)
add_library(parse STATIC parse.c)
target_include_directories(parse PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
								 PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
								 )
target_link_libraries(parse PRIVATE util)


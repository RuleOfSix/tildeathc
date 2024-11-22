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

add_library(il STATIC il.c) 
target_include_directories(il PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
							  PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
							  )
target_link_libraries(il PRIVATE util)


if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
	add_library(util_shared SHARED util.c)
	target_include_directories(util_shared PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
										   PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
										   )

	add_library(tokenize_shared SHARED tokenize.c)
	target_include_directories(tokenize_shared PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc" 
											   PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
											   )

	add_library(parse_shared SHARED parse.c)
	target_include_directories(parse_shared PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
											PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
											)
	target_link_libraries(parse_shared PRIVATE util_shared)

	add_library(il_shared SHARED il.c) 
	target_include_directories(il_shared PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
									     PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
									     )
	target_link_libraries(il_shared PRIVATE util_shared)

endif()


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

add_library(validate_il STATIC validate_il.c)
target_include_directories(validate_il PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
									   PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
									   )
target_link_libraries(validate_il PRIVATE util)

add_library(registers STATIC registers.c)
target_include_directories(registers PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
									 PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
								     )
target_link_libraries(registers PRIVATE util)


add_library(tildeathc_codegen STATIC codegen.c)
target_include_directories(tildeathc_codegen PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
								   PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
								   )
target_link_libraries(tildeathc_codegen PRIVATE registers)

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

	add_library(validate_il_shared SHARED validate_il.c)
	target_include_directories(validate_il_shared PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
												  PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
												  )
	target_link_libraries(validate_il_shared PRIVATE util_shared)

	add_library(registers_shared SHARED registers.c)
	target_include_directories(registers_shared PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
												PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
												)
	target_link_libraries(registers_shared PRIVATE util_shared)


	add_library(tildeathc_codegen_shared SHARED codegen.c)
	target_include_directories(tildeathc_codegen_shared PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
											  PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
											  )
	target_link_libraries(tildeathc_codegen_shared PRIVATE registers_shared)

endif()

add_subdirectory(runtime)

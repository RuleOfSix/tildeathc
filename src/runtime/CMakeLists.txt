cmake_minimum_required(VERSION 3.22)

add_library(objects STATIC objects.c)
target_include_directories(objects PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
								   PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
								   )
target_link_libraries(objects PRIVATE util)


if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
	add_library(objects_shared SHARED objects.c)
	target_include_directories(objects_shared PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
											  PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
											  )
	target_link_libraries(objects_shared PRIVATE util_shared)
endif()


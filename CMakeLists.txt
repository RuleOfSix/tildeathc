cmake_minimum_required(VERSION 3.22)
project(tildeathc VERSION 0.1
		          DESCRIPTION "Implementation of RuleOfSix's version of ~ATH as a compiler in C."
				  LANGUAGES C)

set(CMAKE_C_STANDARD 17)

configure_file("include/tildeathc/project_info.h.in" "${PROJECT_BINARY_DIR}/include/tildeathc/project_info.h")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	include(CTest)
endif()

add_subdirectory(src)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
	add_subdirectory(test)
endif()


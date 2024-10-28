cmake_minimum_required(VERSION 3.22)
project(tildeathc VERSION 0.1
		          DESCRIPTION "Implementation of RuleOfSix's version of ~ATH as a compiler in C."
				  LANGUAGES C)

set(CMAKE_C_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
if (NOT CMAKE_BUILD_TYPE)
	set(CMAKE_BUILD_TYPE Release)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release") 
	add_compile_options(-O3)
endif()
if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_options(-Wall -Wextra -Wpedantic -Werror)
endif()

configure_file("include/tildeathc/project_info.h.in" "${PROJECT_BINARY_DIR}/include/tildeathc/project_info.h")

if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME)
	include(CTest)
endif()

add_subdirectory(src)
if(CMAKE_PROJECT_NAME STREQUAL PROJECT_NAME AND BUILD_TESTING)
	add_subdirectory(test)
endif()


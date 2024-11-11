cmake_minimum_required(VERSION 3.22)

function(tildeathc_add_test args)
	cmake_parse_arguments(PARSE_ARGV 0 TILDEATHC "" "TARGET_NAME;TEST_NAME;SOURCE_FILE;REGEX" "DEPENDENCIES")
	add_executable( ${TILDEATHC_TARGET_NAME} ${TILDEATHC_SOURCE_FILE})
	target_include_directories(${TILDEATHC_TARGET_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
														PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
														PRIVATE "${PROJECT_SOURCE_DIR}/test/include"
														)
	foreach(LIB IN LISTS TILDEATHC_DEPENDENCIES)
		target_link_libraries(${TILDEATHC_TARGET_NAME} PRIVATE ${LIB})
	endforeach()
	target_link_libraries(${TILDEATHC_TARGET_NAME} PRIVATE tildeathc_test)

	add_test(NAME ${TILDEATHC_TEST_NAME} COMMAND ${TILDEATHC_TARGET_NAME})
	if (DEFINED TILDEATHC_REGEX)
		set_property(TEST ${TILDEATHC_TEST_NAME} PROPERTY PASS_REGULAR_EXPRESSION ${TILDEATHC_REGEX})
	endif()
endfunction()

add_library(tildeathc_test STATIC tildeathc_test.c)
target_include_directories(tildeathc_test			PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
													PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
													PRIVATE "${PROJECT_SOURCE_DIR}/test/include"
												    )
target_link_libraries(tildeathc_test tokenize parse il)

add_subdirectory(tokenize)
add_subdirectory(parse)
add_subdirectory(integration)
add_subdirectory(il)

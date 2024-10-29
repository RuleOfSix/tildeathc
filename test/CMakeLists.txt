cmake_minimum_required(VERSION 3.22)

function(tildeathc_add_test args)
	cmake_parse_arguments(PARSE_ARGV 0 TILDEATHC "" "TARGET_NAME;TEST_NAME;SOURCE_FILE;REGEX" "DEPENDENCIES")
	add_executable( ${TILDEATHC_TARGET_NAME} ${TILDEATHC_SOURCE_FILE})
	target_include_directories(${TILDEATHC_TARGET_NAME} PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
											  PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
											  )
	foreach(LIB IN LISTS TILDEATHC_DEPENDENCIES)
		target_link_libraries(${TILDEATHC_TARGET_NAME} PRIVATE ${LIB})
	endforeach()

	add_test(NAME ${TILDEATHC_TEST_NAME} COMMAND ${TILDEATHC_TARGET_NAME})
	if (DEFINED TILDEATHC_REGEX)
		set_property(TEST ${TILDEATHC_TEST_NAME} PROPERTY PASS_REGULAR_EXPRESSION ${TILDEATHC_REGEX})
	endif()
endfunction()

tildeathc_add_test(TARGET_NAME tokenize_test TEST_NAME TokenizeTest SOURCE_FILE tokenize_test.c DEPENDENCIES tokenize)
tildeathc_add_test(TARGET_NAME tokenize_crash TEST_NAME TokenizeCrash SOURCE_FILE tokenize_crash.c DEPENDENCIES tokenize REGEX "malformed character data")
tildeathc_add_test(TARGET_NAME comment_test TEST_NAME CommentTest SOURCE_FILE comment_test.c DEPENDENCIES tokenize)
tildeathc_add_test(TARGET_NAME quote_test TEST_NAME QuoteTest SOURCE_FILE quote_test.c DEPENDENCIES tokenize)
tildeathc_add_test(TARGET_NAME quote_error TEST_NAME QuoteError SOURCE_FILE quote_error.c DEPENDENCIES tokenize REGEX "unterminated string on line 1")
tildeathc_add_test(TARGET_NAME basic_parse_test TEST_NAME BasicParseTest SOURCE_FILE basic_parse_test.c DEPENDENCIES parse)
tildeathc_add_test(TARGET_NAME syntax_error TEST_NAME SyntaxError SOURCE_FILE syntax_error.c DEPENDENCIES parse REGEX "Syntax error on line 1: expected '\\(', got '\\{' instead")
tildeathc_add_test(TARGET_NAME invalid_varname TEST_NAME InvalidVarname SOURCE_FILE invalid_varname.c DEPENDENCIES parse REGEX "Syntax error on line 1: 'NULL' is an invalid variable name")
tildeathc_add_test(TARGET_NAME parse_empty_loop TEST_NAME ParseEmptyLoop SOURCE_FILE parse_empty_loop.c DEPENDENCIES parse)
tildeathc_add_test(TARGET_NAME unfinished_grave TEST_NAME UnfinishedGrave SOURCE_FILE unfinished_grave.c DEPENDENCIES parse REGEX "unexpectedly encountered end of file")
tildeathc_add_test(TARGET_NAME tokenize_parse_integration TEST_NAME TokenParseIntegration SOURCE_FILE tokenize_parse_integration.c DEPENDENCIES parse;tokenize)

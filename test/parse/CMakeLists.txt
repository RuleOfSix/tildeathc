cmake_minimum_required(VERSION 3.22)

tildeathc_add_test(TARGET_NAME basic_parse_test TEST_NAME BasicParseTest SOURCE_FILE basic_parse_test.c DEPENDENCIES parse)
tildeathc_add_test(TARGET_NAME syntax_error TEST_NAME SyntaxError SOURCE_FILE syntax_error.c DEPENDENCIES parse REGEX "Syntax error on line 1: expected '\\(', got '\\{' instead")
tildeathc_add_test(TARGET_NAME invalid_varname TEST_NAME InvalidVarname SOURCE_FILE invalid_varname.c DEPENDENCIES parse REGEX "Syntax error on line 1: 'NULL' is an invalid variable name")
tildeathc_add_test(TARGET_NAME parse_empty_loop TEST_NAME ParseEmptyLoop SOURCE_FILE parse_empty_loop.c DEPENDENCIES parse)
tildeathc_add_test(TARGET_NAME unfinished_grave TEST_NAME UnfinishedGrave SOURCE_FILE unfinished_grave.c DEPENDENCIES parse REGEX "unexpectedly encountered end of file")


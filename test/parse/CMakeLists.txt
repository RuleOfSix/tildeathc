cmake_minimum_required(VERSION 3.22)

tildeathc_add_test(TARGET_NAME parse_empty_loop TEST_NAME ParseEmptyLoop SOURCE_FILE empty_loop.c DEPENDENCIES parse_shared)
tildeathc_add_test(TARGET_NAME parse_imports TEST_NAME ParseImports SOURCE_FILE imports.c DEPENDENCIES parse_shared)
tildeathc_add_test(TARGET_NAME parse_execute_print TEST_NAME ParseExecutePrint SOURCE_FILE execute_print.c DEPENDENCIES parse_shared)
tildeathc_add_test(TARGET_NAME parse_nested TEST_NAME ParseNested SOURCE_FILE nested.c DEPENDENCIES parse_shared)
tildeathc_add_test(TARGET_NAME parse_syntax_error TEST_NAME ParseSyntaxError SOURCE_FILE syntax_error.c DEPENDENCIES parse_shared REGEX "Syntax error on line 1: expected '\\(', got '\\{' instead")
tildeathc_add_test(TARGET_NAME parse_invalid_varname TEST_NAME ParseInvalidVarname SOURCE_FILE invalid_varname.c DEPENDENCIES parse_shared REGEX "Syntax error on line 1: 'NULL' is an invalid variable name")
tildeathc_add_test(TARGET_NAME parse_unrecognized_token TEST_NAME ParseUnrecognizedToken SOURCE_FILE unrecognized_token.c DEPENDENCIES parse_shared REGEX "Syntax error on line 1: unrecognized token 'bad_token'")
tildeathc_add_test(TARGET_NAME parse_banned_varname_unrecognized_token TEST_NAME ParseBannedVarnameUnrecognizedToken SOURCE_FILE banned_varname_unrecognized_token.c DEPENDENCIES parse_shared REGEX "Syntax error on line 1: unrecognized token 'NULL'")
tildeathc_add_test(TARGET_NAME parse_unfinished_grave TEST_NAME ParseUnfinishedGrave SOURCE_FILE unfinished_grave.c DEPENDENCIES parse_shared REGEX "unexpectedly encountered end of file")


cmake_minimum_required(VERSION 3.22)

tildeathc_add_test(TARGET_NAME tokenize_test TEST_NAME TokenizeTest SOURCE_FILE tokenize_test.c DEPENDENCIES tokenize_shared)
tildeathc_add_test(TARGET_NAME tokenize_crash TEST_NAME TokenizeCrash SOURCE_FILE tokenize_crash.c DEPENDENCIES tokenize_shared REGEX "malformed character data")
tildeathc_add_test(TARGET_NAME comment_test TEST_NAME CommentTest SOURCE_FILE comment_test.c DEPENDENCIES tokenize_shared)
tildeathc_add_test(TARGET_NAME quote_test TEST_NAME QuoteTest SOURCE_FILE quote_test.c DEPENDENCIES tokenize_shared)
tildeathc_add_test(TARGET_NAME quote_error TEST_NAME QuoteError SOURCE_FILE quote_error.c DEPENDENCIES tokenize_shared REGEX "unterminated string on line 1")

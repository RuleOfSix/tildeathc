cmake_minimum_required(VERSION 3.22)

add_executable(tokenize_test tokenize_test.c)
target_include_directories(tokenize_test PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
										 PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
										 )
target_link_libraries(tokenize_test PRIVATE tokenize)

add_executable(tokenize_crash tokenize_crash.c)
target_include_directories(tokenize_crash PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
										 PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
										 )
target_link_libraries(tokenize_crash PRIVATE tokenize)

add_executable(comment_test comment_test.c)
target_include_directories(comment_test PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
										PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
										)
target_link_libraries(comment_test PRIVATE tokenize)

add_executable(quote_test quote_test.c)
target_include_directories(quote_test PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
									  PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
									  )
target_link_libraries(quote_test PRIVATE tokenize)

add_executable(quote_error quote_error.c)
target_include_directories(quote_error PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
									   PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
									   )
target_link_libraries(quote_error PRIVATE tokenize)

add_executable(basic_parse_test basic_parse_test.c) 
target_include_directories(basic_parse_test PRIVATE "${PROJECT_SOURCE_DIR}/include/tildeathc"
											PRIVATE "${PROJECT_BINARY_DIR}/include/tildeathc"
											)
target_link_libraries(basic_parse_test PRIVATE parse)

add_test(NAME TokenizeTest COMMAND tokenize_test)
add_test(NAME TokenizeCrash COMMAND tokenize_crash)
set_property(TEST TokenizeCrash PROPERTY PASS_REGULAR_EXPRESSION "malformed character data")
add_test(NAME CommentTest COMMAND comment_test)
add_test(NAME QuoteTest COMMAND quote_test)
add_test(NAME QuoteError COMMAND quote_error)
set_property(TEST QuoteError PROPERTY PASS_REGULAR_EXPRESSION "unterminated string on line 1")
add_test(NAME BasicParseTest COMMAND basic_parse_test)

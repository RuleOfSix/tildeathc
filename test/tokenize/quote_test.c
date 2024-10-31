#include <tildeathc_test.h>
#include <tokenize.h>
#include <project_info.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#ifndef TEST_INPUT_DIR 
#define TEST_INPUT_DIR "."
#endif

int32_t main(void) {
	FILE* test_input = fopen(TEST_INPUT_DIR "/QUOTE_TEST.~ATH", "r");
	if (test_input == NULL) {
		fprintf(stderr, "Test error: failed to open tokenize test input file. Terminating.\n");
		exit(EXIT_FAILURE);
	}

	struct token expected_tokens[] = {
		{"\"This should all be a single token.\"", 1},
		{"~ATH", 1},
		{"(", 1},
		{"}", 1},
		{"PRINT", 1},
		{"\"Even with multiple on one line\"", 1},
		{"\"Or with the #comment marker inside.\"", 2},
		{"~ATH", 3},
		{"\"this too\"", 3},
		{"EXECUTE", 3}
	};

	struct token_list expected_outputs = { expected_tokens, 10 };
	tokenize_test(test_input, &expected_outputs, false);
	printf("Tokenize quote behavior test passed.\n");
	fclose(test_input);
	return 0;
}

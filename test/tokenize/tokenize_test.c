#include <tildeathc_test.h>
#include <tokenize.h>
#include <project_info.h>
#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#ifndef TEST_INPUT_DIR 
#define TEST_INPUT_DIR "."
#endif

int32_t main(void) {
	FILE* test_input = fopen(TEST_INPUT_DIR "/TOKENIZE_TEST.~ATH", "r");
	if (test_input == NULL) {
		fprintf(stderr, "Test error: failed to open tokenize test input file. Terminating.\n");
		exit(EXIT_FAILURE);
	}

	struct token expected_tokens[] = {
        {"import", 1},
		{"library", 1},
		{"NUMBERS", 1},
		{";", 1},
		{"import", 2},
		{"abstract", 2},
		{"LAMB", 2},
		{";", 2},
		{"~ATH", 4},
		{"(", 4},
		{"LAMB", 4},
		{")", 4},
		{"{", 4},
		{"LAMB", 5},
		{".DIE", 5},
		{"(", 5},
		{")", 5},
		{";", 5},
		{"}", 6},
		{"EXECUTE", 6},
		{"(", 6},
		{"PRINT", 6},
		{"\"HELLO WORLD!\"", 6},
		{";", 6},
		{")", 6},
		{";", 6},
		{"THIS", 7},
		{".DIE", 7},
		{"(", 7},
		{")", 7},
		{";", 7}
	};

	struct token_list expected_outputs = { expected_tokens, 31 };
	tokenize_test(test_input, &expected_outputs, false);
	printf("Basic tokenize test passed.\n");
	fclose(test_input);
	return 0;
}

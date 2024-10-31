#include <tildeathc_test.h>
#include <tokenize.h>
#include <project_info.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#ifndef TEST_INPUT_DIR 
#define TEST_INPUT_DIR "."
#endif

int32_t main(void) {
	FILE* test_input = fopen(TEST_INPUT_DIR "/COMMENT_TEST.~ATH", "r");
	if (test_input == NULL) {
		fprintf(stderr, "Test error: failed to open tokenize test input file. Terminating.\n");
		exit(EXIT_FAILURE);
	}

	struct token expected_tokens[] = {
        {"import", 1},
		{"abstract", 1},
		{"CANARY", 1},
		{";", 1},
		{"import", 2},
		{"abstract", 2},
		{"LAMB", 2},
		{";", 2},
		{"~ATH", 6},
		{"(", 6},
		{"LAMB", 6},
		{")", 6},
		{"{", 6},
		{"LAMB", 7},
		{".DIE", 7},
		{"(", 7},
		{")", 7},
		{";", 7},
		{"CANARY", 8},
		{".DIE", 8},
		{"(", 8},
		{")", 8},
		{";", 8},
		{"}", 9},
		{"EXECUTE", 9},
		{"(", 9},
		{"NULL", 9},
		{")", 9},
		{";", 9},
		{"~ATH", 11},
		{"(", 11},
		{"CANARY", 11},
		{")", 11},
		{"{", 11},
		{"CANARY", 12},
		{".DIE", 12},
		{"(", 12},
		{")", 12},
		{";", 12},
		{"CANA", 13},
		{"}", 14},
		{"EXECUTE", 14},
		{"(", 14},
		{"NULL", 14},
		{")", 14},
		{";", 14},
		{"THIS", 17},
		{".DIE", 17},
		{"(", 17},
		{")", 17},
		{";", 17}
	};

	struct token_list expected_outputs = { expected_tokens, 51 };
	tokenize_test(test_input, &expected_outputs, false);
	printf("Tokenize comment behavior test passed\n");
	fclose(test_input);
	return 0;
}

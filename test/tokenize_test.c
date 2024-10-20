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
	FILE* test_input = fopen(TEST_INPUT_DIR "/TOKENIZE_TEST.~ATH", "r");
	if (test_input == NULL) {
		fprintf(stderr, "Test error: failed to open tokenize test input file. Terminating.\n");
		exit(EXIT_FAILURE);
	}
	bool passed = true;

	token expected_tokens[] = {
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
		{"\"", 6},
		{"HELLO", 6},
		{"WORLD!", 6},
		{"\"", 6},
		{";", 6},
		{")", 6},
		{";", 6},
		{"THIS", 7},
		{".DIE", 7},
		{"(", 7},
		{")", 7},
		{";", 7}
	};

	token_list expected_outputs = { expected_tokens, 34 };
	token_list* received_output = tokenize(test_input);
	fclose(test_input);
	if (received_output == NULL) {
		fprintf(stderr, "Test error: tokenizer output null. Terminating.\n");
		exit(EXIT_FAILURE);
	}
	
	if (received_output->length != expected_outputs.length) {
		fprintf(stderr, "Test failed: expected length property of %ld, output was %ld. Terminating.\n", expected_outputs.length, received_output->length);
		exit(EXIT_FAILURE);
	}

	for (int64_t i = 0; i < received_output->length; i++) {
		if (strcmp(received_output->tokens[i].str, expected_outputs.tokens[i].str) != 0) {
			fprintf(stderr, "Test failed: expected token %ld's str to be %s, output was %s.\n", i, expected_outputs.tokens[i].str, received_output->tokens[i].str);
			passed = false;
		}
		if (received_output->tokens[i].lineno != expected_outputs.tokens[i].lineno) {
			fprintf(stderr, "Test failed: expected token %ld's lineno to be %ld, output was %ld.\n", i, expected_outputs.tokens[i].lineno, received_output->tokens[i].lineno);
			passed = false;
		}
	}
	 
	if (passed) {
		printf("Tokenizer module passed all tests.\n");
	}
	free_token_list(received_output);
	free(received_output);
	return 0;
}

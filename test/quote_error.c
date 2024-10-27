#include <tokenize.h>
#include <project_info.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#ifndef TEST_INPUT_DIR 
#define TEST_INPUT_DIR "."
#endif

int32_t main(void) {
	FILE* test_input = fopen(TEST_INPUT_DIR "/QUOTE_ERROR.~ATH", "r");
	if (test_input == NULL) {
		fprintf(stderr, "Test error: failed to open tokenize test input file. Terminating.\n");
		exit(EXIT_SUCCESS); // This test is supposed to fail, so it needs to exit "successfully" if something weird happens
	}
	tokenize(test_input);
	fprintf(stderr, "Error: unterminated string did not cause tokenizer error.\n");
	exit(EXIT_SUCCESS);
}

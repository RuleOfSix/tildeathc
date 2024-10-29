#include <tokenize.h>
#include <parse.h>
#include <project_info.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

int32_t main() {
	struct token input_tokens[] = {
		{"~ATH", 1},
		{"(", 1},
	};
	struct token_list input = {input_tokens, 2};

	parse(&input);
	fprintf(stderr, "Test failed: parser did not throw a syntax error when given incorrect input.\n");
	return 0;
}


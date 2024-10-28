#include <parse.h>
#include <project_info.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

int32_t main() {
	token input_tokens[] = {
		{"~ATH", 1},
		{"(", 1},
		{"NULL", 1},
		{")", 1},
		{"{", 1},
		{"}", 2},
		{"EXECUTE", 2},
		{"(", 2},
		{"NULL", 2},
		{")", 2},
		{";", 2}
	};
	token_list input = {input_tokens, 12};

	parse(&input);
	fprintf(stderr, "Test failed: parser did not throw a syntax error when given incorrect input.\n");
	return 0;
}


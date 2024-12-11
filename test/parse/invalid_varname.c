#include <tildeathc_test.h>
#include <tokenize.h>
#include <parse.h>
#include <project_info.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

int32_t main(void) {
	struct token input_tokens[] = {
		{"import", 1},
		{"abstract", 1},
		{"NULL", 1},
		{";", 1},
	};
	struct token_list input = {input_tokens, 4};

	parse_test(&input, NULL, true);	
	return 0;
}


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
		{"~ATH", 1},
		{"(", 1},
		{"LAMB", 1},
		{")", 1},
		{"{", 1},
		{"}", 1},
		{"EXECUTE", 1},
		{"(", 1},
		{"NULL", 1},
		{")", 1},
		{";", 1},
		{"THIS", 2},
		{".DIE", 2},
		{"(", 2},
		{")", 2},
		{";", 2}
	};
	struct token_list input = {input_tokens, 16};

	struct ast loop_c[] = {{.type=STRING_NODE, .val.str="LAMB", .lineno=1, .num_children=0, .children=NULL},
						  {.type=OPERATION_NODE, .val.op=NULL_OP, .lineno=1, .num_children=0, .children=NULL}};
	struct ast death_c[] = {{.type=STRING_NODE, .val.str="THIS", .lineno=2, .num_children=0, .children=NULL}};
	struct ast root_c[] = {{.type=OPERATION_NODE, .val.op=LOOP_OP, .lineno=1, .num_children=2, .children=loop_c},
						{.type=OPERATION_NODE, .val.op=DIE_OP, .lineno=2, .num_children=1, .children=death_c}};
	struct ast expected_output = {.type=ROOT_NODE, .val.str=NULL, .lineno=0, .num_children=2, .children=root_c};

	parse_test(&input, &expected_output, false);
	printf("Parse module empty loop test passed\n");
	return 0;
}


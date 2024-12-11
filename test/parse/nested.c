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
		{"~ATH", 2},
		{"(", 2},
		{"LAMB", 2},
		{")", 2},
		{"{", 2},
		{"}", 2},
		{"EXECUTE", 2},
		{"(", 2},
		{"NULL", 2},
		{")", 2},
		{";", 2},
		{")", 3},
		{";", 3},
		{"THIS", 4},
		{".DIE", 4},
		{"(", 4},
		{")", 4},
		{";", 4}
	};
	struct token_list input = {input_tokens, 26};

	
	struct ast expected_output = {.type=ROOT_NODE, .val.str=NULL, .lineno=0, .num_children=2, .children=(struct ast[]) {
									{.type=OPERATION_NODE, .val.op=LOOP_OP, .lineno=1, .num_children=2, .children=(struct ast[]) {
										{.type=STRING_NODE, .val.str="LAMB", .lineno=1, .num_children=0, .children=NULL},
										{.type=OPERATION_NODE, .val.op=LOOP_OP, .lineno=2, .num_children=2, .children=(struct ast[]) {
											{.type=STRING_NODE,.val.str="LAMB", .lineno=2, .num_children=0, .children=NULL},
											{.type=OPERATION_NODE,.val.op=NULL_OP, .lineno=2, .num_children=0, .children=NULL}}}}},
									{.type=OPERATION_NODE, .val.op=DIE_OP, .lineno=4, .num_children=1, .children=(struct ast[]) {
										{.type=STRING_NODE, .val.str="THIS", .lineno=4, .num_children=0, .children=NULL}}}}};

	parse_test(&input, &expected_output, false);
	printf("Parse module empty loop test passed\n");
	return 0;
}


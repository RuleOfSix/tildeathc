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
		{"LAMB", 1},
		{";", 1},
		{"import", 2},
		{"input", 2},
		{"IN", 2},
		{";", 2},
		{"import", 3},
		{"library", 3},
		{"NUMBERS", 3},
		{";", 3},
		{"import", 4},
		{"universe", 4},
		{"U1", 4},
		{";", 4},
		{"THIS", 5},
		{".DIE", 5},
		{"(", 5},
		{")", 5},
		{";", 5}
	};
	struct token_list input = {input_tokens, 21};

	struct ast expected_output = {.type=ROOT_NODE, .val.str=NULL, .lineno=0, .num_children=5, .children=(struct ast[]) {
									{.type=OPERATION_NODE, .val.op=IMPORT_OP, .lineno=1, .num_children=2, .children=(struct ast[]) {
										{.type=STRING_NODE, .val.str="abstract", .lineno=1, .num_children=0, .children=NULL},
										{.type=STRING_NODE, .val.str="LAMB", .lineno=1, .num_children=0, .children=NULL}}},
									{.type=OPERATION_NODE, .val.op=IMPORT_OP, .lineno=2, .num_children=2, .children=(struct ast[]) {
										{.type=STRING_NODE, .val.str="input", .lineno=2, .num_children=0, .children=NULL},
										{.type=STRING_NODE, .val.str="IN", .lineno=2, .num_children=0, .children=NULL}}},
									{.type=OPERATION_NODE, .val.op=IMPORT_OP, .lineno=3, .num_children=2, .children=(struct ast[]) {
										{.type=STRING_NODE, .val.str="library", .lineno=3, .num_children=0, .children=NULL},
										{.type=STRING_NODE, .val.str="NUMBERS", .lineno=3, .num_children=0, .children=NULL}}},
									{.type=OPERATION_NODE, .val.op=IMPORT_OP, .lineno=4, .num_children=2, .children=(struct ast[]) {
										{.type=STRING_NODE, .val.str="universe", .lineno=4, .num_children=0, .children=NULL},
										{.type=STRING_NODE, .val.str="U1", .lineno=4, .num_children=0, .children=NULL}}},
									{.type=OPERATION_NODE, .val.op=DIE_OP, .lineno=5, .num_children=1, .children=(struct ast[]) {
										{.type=STRING_NODE, .val.str="THIS", .lineno=5, .num_children=0, .children=NULL}}}}};

	parse_test(&input, &expected_output, false);
	printf("Parse module import test passed\n");
	return 0;
}


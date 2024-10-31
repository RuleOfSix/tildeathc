#include <tildeathc_test.h>
#include <tokenize.h>
#include <parse.h>
#include <project_info.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

bool compare(struct ast* expected, struct ast* actual);

int32_t main() {
	struct token input_tokens[] = {
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
	struct token_list input = {input_tokens, 31};

	struct ast grave_1_c[] = {{.type=STRING_NODE, .val.str="library", .lineno=1, .num_children=0, .children=NULL},
					   {.type=STRING_NODE, .val.str="NUMBERS", .lineno=1, .num_children=0, .children=NULL}};
	struct ast grave_1 = {.type=OPERATION_NODE, .val.op=IMPORT_OP, .lineno=1, .num_children=2, .children=grave_1_c};
	struct ast grave_2_c[] = {{.type=STRING_NODE, .val.str="abstract", .lineno=2, .num_children=0, .children=NULL},
					   {.type=STRING_NODE, .val.str="LAMB", .lineno=2, .num_children=0, .children=NULL}};
	struct ast grave_2 = {.type=OPERATION_NODE, .val.op=IMPORT_OP, .lineno=2, .num_children=2, .children=grave_2_c};
	struct ast grave_3_c_2_c[] = {{.type=STRING_NODE, .val.str="LAMB", .lineno=5, .num_children=0, .children=NULL}};
	struct ast grave_3_c_3_c[] = {{.type=STRING_NODE, .val.str="HELLO WORLD!", .lineno=6, .num_children=0, .children=NULL}};
	struct ast grave_3_c[] = {{.type=STRING_NODE, .val.str="LAMB", .lineno=4, .num_children=0, .children=NULL},
					 {.type=OPERATION_NODE, .val.op=DIE_OP, .lineno=5, .num_children=1, .children=grave_3_c_2_c},
					 {.type=OPERATION_NODE, .val.op=PRINT_OP, .lineno=6, .num_children=1, .children=grave_3_c_3_c}};
	struct ast grave_3 = {.type=OPERATION_NODE, .val.op=LOOP_OP, .lineno=4, .num_children=3, .children=grave_3_c};
	struct ast grave_4_c[] = {{.type=STRING_NODE, .val.str="THIS", .lineno=7, .num_children=0, .children=NULL}};
	struct ast grave_4 = {.type=OPERATION_NODE, .val.op=DIE_OP, .lineno=7, .num_children=1, .children=grave_4_c};
	struct ast root_c[] = {grave_1, grave_2, grave_3, grave_4};
	struct ast expected_output = {.type=ROOT_NODE, .val.str=NULL, .lineno=0, .num_children=4, .children=root_c};

	parse_test(&input, &expected_output, false);
	printf("Basic parse module test passed\n");
	return 0;
}


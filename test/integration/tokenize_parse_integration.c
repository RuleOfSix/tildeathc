#include <tildeathc_test.h>
#include <project_info.h>
#include <tokenize.h>
#include <parse.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

bool compare(struct ast* expected, struct ast* actual);

#ifndef TEST_INPUT_DIR 
#define TEST_INPUT_DIR "."
#endif

int32_t main(void) {
	FILE* input = fopen(TEST_INPUT_DIR "/TOKENIZE_PARSE_INTEGRATION.~ATH", "r");
	if (input == NULL) {
		fprintf(stderr, "Unable to open test input file. Terminating\n");
		exit(EXIT_FAILURE);
	}

	struct ast grave_1_c[] = {{.type=STRING_NODE, .val.str="library", .lineno=1, .num_children=0, .children=NULL},
						      {.type=STRING_NODE, .val.str="NUMBERS", .lineno=1, .num_children=0, .children=NULL}};
	struct ast grave_2_c[] = {{.type=STRING_NODE, .val.str="abstract", .lineno=2, .num_children=0, .children=NULL},
							  {.type=STRING_NODE, .val.str="LAMB", .lineno=2, .num_children=0, .children=NULL}};
	struct ast grave_3_c[] = {{.type=STRING_NODE, .val.str="N7", .lineno=3, .num_children=0, .children=NULL},
							  {.type=STRING_NODE, .val.str="NUM", .lineno=3, .num_children=0, .children=NULL},
							  {.type=STRING_NODE, .val.str="NUL", .lineno=3, .num_children=0, .children=NULL}};
	struct ast grave_4_2_c[] = {{.type=STRING_NODE, .val.str="NUM", .lineno=5, .num_children=0, .children=NULL},
								{.type=STRING_NODE, .val.str="NUM2", .lineno=5, .num_children=0, .children=NULL},
								{.type=STRING_NODE, .val.str="NUL", .lineno=5, .num_children=0, .children=NULL}};
	struct ast grave_4_3_c[] = {{.type=STRING_NODE, .val.str="LAMB", .lineno=6, .num_children=0, .children=NULL}};
	struct ast grave_4_4_c[] = {{.type=STRING_NODE, .val.str="DEC", .lineno=7, .num_children=0, .children=NULL}};
	struct ast grave_4_c[] = {{.type=STRING_NODE, .val.str="LAMB", .lineno=4, .num_children=0, .children=NULL},
							  {.type=OPERATION_NODE, .val.op=BIFURCATE_OP, .lineno=5, .num_children=3, .children=grave_4_2_c},
							  {.type=OPERATION_NODE, .val.op=DIE_OP, .lineno=6, .num_children=1, .children=grave_4_3_c},
							  {.type=OPERATION_NODE, .val.op=PRINT_OP, .lineno=7, .num_children=1, .children=grave_4_4_c}};
	struct ast grave_5_c[] = {{.type=STRING_NODE, .val.str="NUM2", .lineno=8, .num_children=0, .children=NULL},
							  {.type=STRING_NODE, .val.str="NUM", .lineno=8, .num_children=0, .children=NULL}};
	struct ast grave_6_c[] = {{.type=STRING_NODE, .val.str="THIS", .lineno=9, .num_children=0, .children=NULL}};
	struct ast root_c[] = {{.type=OPERATION_NODE, .val.op=IMPORT_OP, .lineno=1, .num_children=2, .children=grave_1_c},
						   {.type=OPERATION_NODE, .val.op=IMPORT_OP, .lineno=2, .num_children=2, .children=grave_2_c},
						   {.type=OPERATION_NODE, .val.op=BIFURCATE_OP, .lineno=3, .num_children=3, .children=grave_3_c},
						   {.type=OPERATION_NODE, .val.op=LOOP_OP, .lineno=4, .num_children=4, .children=grave_4_c},
						   {.type=OPERATION_NODE, .val.op=DIE_OP, .lineno=8, .num_children=2, .children=grave_5_c},
						   {.type=OPERATION_NODE, .val.op=DIE_OP, .lineno=9, .num_children=1, .children=grave_6_c}};
	struct ast expected_output = {.type=ROOT_NODE, .val.str=NULL, .lineno=0, .num_children=6, .children=root_c};
	
	struct ast* output = parse(tokenize(input));
	if (output == NULL) {
		fprintf(stderr, "Test failed: parse output null.\n");
		exit(EXIT_FAILURE);
	}
	if (!compare_ast(&expected_output, output)) {
		fprintf(stderr, "Test failed: parse output does not match expected AST.\n");
		exit(EXIT_FAILURE);
	}
	printf("Integration test passed\n");
	free_ast(output);
	free(output);
	return 0;
}


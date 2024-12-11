#include <parse.h>
#include <il.h>
#include <tildeathc_test.h>
#include <stdint.h>

int32_t main(void) {
	struct ast input =	{.type=ROOT_NODE, .val.str=NULL, .lineno=0, .num_children=1, .children = (struct ast[]) {
							{.type=OPERATION_NODE, .val.op=IMPORT_OP, .lineno=1, .num_children=2, .children = (struct ast[]) {
								{.type=STRING_NODE, .val.str="library", .lineno=1, .num_children=0, .children=NULL},
								{.type=STRING_NODE, .val.str="NUMBERS", .lineno=1, .num_children=0, .children=NULL}}}}};

	struct il_node expected_output = {.type=IL_ROOT_NODE, .val.str=NULL, .id=0, .lineno=0, .num_children=1, .children= (struct il_node[]) {
										{.type=IL_OP_NODE, .val.op=IL_LIB_OP, .id=1, .lineno=1, .num_children=1, .children=(struct il_node[]) {
											{.type=IL_DEC_NODE, .val.str="NUMBERS", .id=2, .lineno=1, .num_children=0, .children=NULL}}}}};
	
	il_test(&input, &expected_output, false);
	printf("Test of AST-to-IL conversion of library node passed.");
	return 0;
}

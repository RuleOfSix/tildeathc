#include <parse.h>
#include <il.h>
#include <tildeathc_test.h>
#include <stdint.h>

int32_t main(void) {
	struct ast input =	{.type=ROOT_NODE, .val.str=NULL, .lineno=0, .num_children=1, .children = (struct ast[]) {
							{.type=OPERATION_NODE, .val.op=DIE_OP, .lineno=1, .num_children=3, .children = (struct ast[]) {
								{.type=STRING_NODE, .val.str="LAMB1", .lineno=1, .num_children=0, .children=NULL},
								{.type=STRING_NODE, .val.str="LAMB2", .lineno=1, .num_children=0, .children=NULL},
								{.type=STRING_NODE, .val.str="LAMB3", .lineno=1, .num_children=0, .children=NULL}}}}};

	struct il_node expected_output = {.type=IL_ROOT_NODE, .val.str=NULL, .id=0, .lineno=0, .num_children=1, .children= (struct il_node[]) {
										{.type=IL_OP_NODE, .val.op=IL_DIE_OP, .id=1, .lineno=1, .num_children=3, .children=(struct il_node[]) {
											{.type=IL_VAR_NODE, .val.str="LAMB1", .id=2, .lineno=1, .num_children=0, .children=NULL},
											{.type=IL_VAR_NODE, .val.str="LAMB2", .id=3, .lineno=1, .num_children=0, .children=NULL},
											{.type=IL_VAR_NODE, .val.str="LAMB3", .id=4, .lineno=1, .num_children=0, .children=NULL}}}}};
	
	il_test(&input, &expected_output, false);
	printf("Test of AST-to-IL conversion of die node with multiple children passed.");
	return 0;
}

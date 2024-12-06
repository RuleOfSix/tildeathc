#include <il.h>
#include <validate_il.h>
#include <stdio.h>
#include <stdlib.h>

int32_t main(void) {
	struct il_node input =	{.type=IL_ROOT_NODE, .val.str=NULL, .id=0, .lineno=0, .num_children=2, .children=(struct il_node[]) {
								{.type=IL_OP_NODE, .val.op=IL_ABS_OP, .id=1, .lineno=1, .num_children=1, .children=(struct il_node[]) {
									{.type=IL_DEC_NODE, .val.str="LAMB", .id=2, .lineno=1, .num_children=0, .children=NULL}}},
								{.type=IL_OP_NODE, .val.op=IL_DIE_OP, .id=3, .lineno=2, .num_children=1, .children=(struct il_node[]) {
									{.type=IL_VAR_NODE, .val.str="LAMB", .id=4, .lineno=2, .num_children=0, .children=NULL}}}}};
	struct varlist* vars = NULL;
	if (validate_il(&input)) {
		fprintf(stderr, "Standard IL tree validation test successful.\n");
		exit(EXIT_SUCCESS); 
	} else {
		fprintf(stderr, "Standard IL tree validation test failed.\n");
		exit(EXIT_FAILURE);
	}
}

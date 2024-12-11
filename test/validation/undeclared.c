#include <il.h>
#include <validate_il.h>
#include <stdio.h>
#include <stdlib.h>

int32_t main(void) {
	struct il_node input =	{.type=IL_ROOT_NODE, .val.str=NULL, .id=0, .lineno=0, .num_children=1, .children= (struct il_node[]) {
								{.type=IL_OP_NODE, .val.op=IL_DIE_OP, .id=1, .lineno=1, .num_children=1, .children= (struct il_node[]) {
									{.type=IL_VAR_NODE, .val.str="LAMB", .id=2, .lineno=1, .num_children=0, .children=NULL}}}}};
	if (validate_il(&input)) {
		fprintf(stderr, "IL tree containing references to undeclared variables was erroneously validated.\n");
		exit(EXIT_SUCCESS); //This test is expected to fail, so it must "succeed" on error.
	} else {
		fprintf(stderr, "Test of invalidation of IL tree with undeclared variables passed.\n");
		exit(EXIT_FAILURE);
	}
}

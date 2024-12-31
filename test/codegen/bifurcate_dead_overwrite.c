#include <il.h>
#include <tildeathc_test.h>

int main(void) {
	struct il_node input =	{.type=IL_ROOT_NODE, .val.str=NULL, .id=0, .lineno=0, .num_children=5, .children=(struct il_node[]) {
								{.type=IL_OP_NODE, .val.op=IL_ABS_OP, .id=1, .lineno=1, .num_children=1, .children=(struct il_node[]) {
									{.type=IL_DEC_NODE, .val.str="COW", .id=2, .lineno=1, .num_children=0, .children=NULL}}},
								{.type=IL_OP_NODE, .val.op=IL_ABS_OP, .id=3, .lineno=2, .num_children=1, .children=(struct il_node[]) {
									{.type=IL_DEC_NODE, .val.str="LAMB", .id=4, .lineno=2, .num_children=0, .children=NULL}}},
								{.type=IL_OP_NODE, .val.op=IL_DIE_OP, .id=5, .lineno=3, .num_children=1, .children=(struct il_node[]) {
									{.type=IL_VAR_NODE, .val.str="LAMB", .id=6, .lineno=3, .num_children=0, .children=NULL}}},
								{.type=IL_OP_NODE, .val.op=IL_IF_OP, .id=7, .lineno=4, .num_children=2, .children=(struct il_node[]) {
									{.type=IL_VAR_NODE, .val.str="COW", .id=8, .lineno=4, .num_children=0, .children=NULL},
									{.type=IL_OP_NODE, .val.op=IL_BLOCK_OP, .id=9, .lineno=4, .num_children=3, .children=(struct il_node[]) {
										{.type=IL_OP_NODE, .val.op=IL_BIF_OP, .id=10, .lineno=5, .num_children=3, .children=(struct il_node[]) {
											{.type=IL_VAR_NODE, .val.str="LAMB", .id=11, .lineno=5, .num_children=0, .children=NULL},
											{.type=IL_DEC_NODE, .val.str="COW", .id=12, .lineno=5, .num_children=0, .children=NULL},
											{.type=IL_DEC_NODE, .val.str="NUL", .id=13, .lineno=5, .num_children=0, .children=NULL}}},
										{.type=IL_OP_NODE, .val.op=IL_IF_OP, .id=14, .lineno=4, .num_children=2, .children=(struct il_node[]) {
											{.type=IL_VAR_NODE, .val.str="COW", .id=15, .lineno=4, .num_children=0, .children=NULL},
											{.type=IL_OP_NODE, .val.op=IL_JMP_OP, .id=16, .lineno=6, .num_children=1, .children=(struct il_node[]) {
												{.type=IL_TAR_NODE, .val.tar=9, .id=17, .lineno=6, .num_children=0, .children=NULL}}}}},
										{.type=IL_OP_NODE, .val.op=IL_OUT_OP, .id=18, .lineno=6, .num_children=1, .children=(struct il_node[]) {
											{.type=IL_STR_NODE,.val.str="COW is dead\n", .id=19, .lineno=6, .num_children=0,.children=NULL}}}}}}},
								{.type=IL_OP_NODE, .val.op=IL_DIE_OP, .id=20, .lineno=7, .num_children=1, .children=(struct il_node[]) {
									{.type=IL_VAR_NODE, .val.str="THIS", .id=21, .lineno=7, .num_children=0, .children=NULL}}}}};

	codegen_test(&input, "COW is dead", false);
}

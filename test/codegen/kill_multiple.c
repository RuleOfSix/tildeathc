#include <il.h>
#include <tildeathc_test.h>

int main(void) {
	struct il_node input =	{.type=IL_ROOT_NODE, .val.str=NULL, .id=0, .lineno=0, .num_children=7, .children=(struct il_node[]) {
								{.type=IL_OP_NODE, .val.op=IL_ABS_OP, .id=1, .lineno=3, .num_children=1, .children=(struct il_node[]) {
									{.type=IL_DEC_NODE, .val.str="A", .id=2, .lineno=3, .num_children=0, .children=NULL}}},
								{.type=IL_OP_NODE, .val.op=IL_BIF_OP, .id=3, .lineno=4, .num_children=3, .children=(struct il_node[]) {
									{.type=IL_VAR_NODE, .val.str="A", .id=4, .lineno=4, .num_children=0, .children=NULL},
									{.type=IL_DEC_NODE, .val.str="B", .id=5, .lineno=4, .num_children=0, .children=NULL},
									{.type=IL_DEC_NODE, .val.str="C", .id=6, .lineno=4, .num_children=0, .children=NULL}}},
								{.type=IL_OP_NODE, .val.op=IL_DIE_OP, .id=7, .lineno=5, .num_children=2, .children=(struct il_node[]) {
									{.type=IL_VAR_NODE, .val.str="B", .id=8, .lineno=5, .num_children=0, .children=NULL},
									{.type=IL_VAR_NODE, .val.str="C", .id=9, .lineno=5, .num_children=0, .children=NULL}}},
								{.type=IL_OP_NODE, .val.op=IL_ABS_OP, .id=10, .lineno=7, .num_children=1, .children=(struct il_node[]) {
									{.type=IL_DEC_NODE, .val.str="CANARY", .id=11, .lineno=7, .num_children=0, .children=NULL}}},
								{.type=IL_OP_NODE, .val.op=IL_IF_OP, .id=12, .lineno=8, .num_children=2, .children=(struct il_node[]) {
									{.type=IL_VAR_NODE, .val.str="B", .id=13, .lineno=8, .num_children=0, .children=NULL},
									{.type=IL_OP_NODE, .val.op=IL_BLOCK_OP, .id=14, .lineno=8, .num_children=3, .children=(struct il_node[]) {
										{.type=IL_OP_NODE, .val.op=IL_DIE_OP, .id=15, .lineno=9, .num_children=1, .children=(struct il_node[]) {
											{.type=IL_VAR_NODE, .val.str="B", .id=16, .lineno=9, .num_children=0, .children=NULL}}},
										{.type=IL_OP_NODE, .val.op=IL_IF_OP, .id=17, .lineno=10, .num_children=2, .children=(struct il_node[]) {
											{.type=IL_VAR_NODE, .val.str="B", .id=18, .lineno=10, .num_children=0, .children=NULL},
											{.type=IL_OP_NODE, .val.op=IL_JMP_OP, .id=19, .lineno=10, .num_children=1, .children=(struct il_node[]) {
												{.type=IL_TAR_NODE, .val.tar=14, .id=20, .lineno=10, .num_children=0, .children=NULL}}}}},
										{.type=IL_OP_NODE, .val.op=IL_DIE_OP, .id=21, .lineno=10, .num_children=1, .children=(struct il_node[]) {
											{.type=IL_VAR_NODE, .val.str="CANARY", .id=22, .lineno=10, .num_children=0, .children=NULL}}}}}}},
								{.type=IL_OP_NODE, .val.op=IL_IF_OP, .id=23, .lineno=11, .num_children=2, .children=(struct il_node[]) {
									{.type=IL_VAR_NODE, .val.str="CANARY", .id=24, .lineno=11, .num_children=0, .children=NULL},
									{.type=IL_OP_NODE, .val.op=IL_BLOCK_OP, .id=25, .lineno=11, .num_children=3, .children=(struct il_node[]) {
										{.type=IL_OP_NODE, .val.op=IL_DIE_OP, .id=26, .lineno=12, .num_children=1, .children=(struct il_node[]) {
											{.type=IL_VAR_NODE, .val.str="CANARY", .id=27, .lineno=12, .num_children=0, .children=NULL}}},
										{.type=IL_OP_NODE, .val.op=IL_IF_OP, .id=28, .lineno=13, .num_children=2, .children=(struct il_node[]) {
											{.type=IL_VAR_NODE, .val.str="CANARY", .id=29, .lineno=13, .num_children=0, .children=NULL},
											{.type=IL_OP_NODE, .val.op=IL_JMP_OP, .id=30, .lineno=13, .num_children=1, .children=(struct il_node[]) {
												{.type=IL_TAR_NODE, .val.tar=25, .id=31, .lineno=13, .num_children=0, .children=NULL}}}}},
										{.type=IL_OP_NODE, .val.op=IL_OUT_OP, .id=32, .lineno=13, .num_children=1, .children=(struct il_node[]) {
											{.type=IL_STR_NODE, .val.str="Nailed it", .id=33, .lineno=13, .num_children=0, .children=NULL}}}}}}},
								{.type=IL_OP_NODE, .val.op=IL_DIE_OP, .id=34, .lineno=15, .num_children=1, .children=(struct il_node[]) {
									{.type=IL_VAR_NODE, .val.str="THIS", .id=35, .lineno=15, .num_children=0, .children=NULL}}}}};

	codegen_test(&input, "Nailed it", false);
}

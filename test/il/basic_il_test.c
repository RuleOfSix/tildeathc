#include <parse.h>
#include <il.h>
#include <tildeathc_test.h>
#include <stdint.h>

int32_t main(void) {
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
	struct ast input = {.type=ROOT_NODE, .val.str=NULL, .lineno=0, .num_children=4, .children=root_c};	

	struct il_node expected_output = {.type=IL_ROOT_NODE, .val.str=NULL, .id=0, .lineno=0, .num_children=4, .children= (struct il_node[]) {
										{.type=IL_OP_NODE, .val.op=IL_LIB_OP, .id=1, .lineno=1, .num_children=1, .children=(struct il_node[]) {
											{.type=IL_DEC_NODE, .val.str="NUMBERS", .id=2, .lineno=1, .num_children=0, .children=NULL}}},
										{.type=IL_OP_NODE, .val.op=IL_ABS_OP, .id=3, .lineno=2, .num_children=1, .children=(struct il_node[]) {
											{.type=IL_DEC_NODE, .val.str="LAMB", .id=4, .lineno=2, .num_children=0, .children=NULL}}},
										{.type=IL_OP_NODE, .val.op=IL_IF_OP, .id=5, .lineno=4, .num_children=2, .children=(struct il_node[]) {
											{.type=IL_VAR_NODE, .val.str="LAMB", .id=6, .lineno=4, .num_children=0, .children=NULL},
											{.type=IL_OP_NODE, .val.op=IL_BLOCK_OP, .id=7, .lineno=4, .num_children=3, .children=(struct il_node[]) {
												{.type=IL_OP_NODE, .val.op=IL_DIE_OP, .id=8, .lineno=5, .num_children=1, .children=(struct il_node[]) {
													{.type=IL_VAR_NODE, .val.str="LAMB", .id=9, .lineno=5, .num_children=0, .children=NULL}}},
												{.type=IL_OP_NODE, .val.op=IL_IF_OP, .id=10, .lineno=6, .num_children=2, .children=(struct il_node[]) {
													{.type=IL_VAR_NODE, .val.str="LAMB", .id=11, .lineno=6, .num_children=0, .children=NULL},
													{.type=IL_OP_NODE, .val.op=IL_JMP_OP, .id=12, .lineno=6, .num_children=1, .children=(struct il_node[]) {
														{.type=IL_TAR_NODE, .val.tar=7, .id=13, .lineno=6, .num_children=0, .children=NULL}}}}},
												{.type=IL_OP_NODE, .val.op=IL_OUT_OP, .id=14, .lineno=6, .num_children=1, .children=(struct il_node[]) {
													{.type=IL_STR_NODE, .val.str="HELLO WORLD!", .id=15, .lineno=6, .num_children=0, .children=NULL}}}}}}},
										{.type=IL_OP_NODE, .val.op=IL_DIE_OP, .id=16, .lineno=7, .num_children=1, .children=(struct il_node[]) {
											{.type=IL_VAR_NODE, .val.str="THIS", .id=17, .lineno=7, .num_children=0, .children=NULL}}}}};

	
	il_test(&input, &expected_output, false);
	printf("Basic AST-to-IL conversion test passed.");
	return 0;
}

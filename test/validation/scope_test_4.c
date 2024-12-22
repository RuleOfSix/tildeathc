#include <il.h>
#include <validate_il.h>
#include <stdio.h>
#include <stdlib.h>
	
int main(void) {
	struct il_node input =  {.type=IL_ROOT_NODE, .val.str=NULL, .id=0, .lineno=0, .num_children=5, .children=(struct il_node[]) {
								{.type=IL_OP_NODE, .val.op=IL_ABS_OP, .id=1, .lineno=1, .num_children=1, .children=(struct il_node[]) {
									{.type=IL_DEC_NODE, .val.str="LAMB", .id=2, .lineno=1, .num_children=0, .children=NULL}}},
								{.type=IL_OP_NODE, .val.op=IL_IF_OP, .id=3, .lineno=2, .num_children=2, .children=(struct il_node[]) {
									{.type=IL_VAR_NODE, .val.str="LAMB", .id=4, .lineno=2, .num_children=0, .children=NULL},
									{.type=IL_OP_NODE, .val.op=IL_BLOCK_OP, .id=5, .lineno=2, .num_children=3, .children=(struct il_node[]) {
										{.type=IL_OP_NODE, .val.op=IL_ABS_OP, .id=6, .lineno=3, .num_children=1, .children=(struct il_node[]) {
											{.type=IL_DEC_NODE, .val.str="BUTTERFLY", .id=7, .lineno=3, .num_children=0, .children=NULL}}},
										{.type=IL_OP_NODE, .val.op=IL_DIE_OP, .id=8, .lineno=4, .num_children=1, .children=(struct il_node[]) {
											{.type=IL_VAR_NODE, .val.str="LAMB", .id=9, .lineno=4, .num_children=0, .children=NULL}}},
										{.type=IL_OP_NODE, .val.op=IL_IF_OP, .id=10, .lineno=5, .num_children=2, .children=(struct il_node[]) {
											{.type=IL_VAR_NODE, .val.str="LAMB", .id=11, .lineno=5, .num_children=0, .children=NULL},
											{.type=IL_OP_NODE, .val.op=IL_JMP_OP, .id=12, .lineno=5, .num_children=1, .children=(struct il_node[]) {
												{.type=IL_TAR_NODE, .val.tar=5, .id=13, .lineno=5, .num_children=0, .children=NULL}}}}}}}}},
								{.type=IL_OP_NODE, .val.op=IL_ABS_OP, .id=14, .lineno=7, .num_children=1, .children=(struct il_node[]) {
									{.type=IL_DEC_NODE, .val.str="LAMB", .id=15, .lineno=7, .num_children=0, .children=NULL}}},
								{.type=IL_OP_NODE, .val.op=IL_IF_OP, .id=16, .lineno=8, .num_children=2, .children=(struct il_node[]) {
									{.type=IL_VAR_NODE, .val.str="LAMB", .id=17, .lineno=8, .num_children=0, .children=NULL},
									{.type=IL_OP_NODE, .val.op=IL_BLOCK_OP, .id=18, .lineno=8, .num_children=2, .children=(struct il_node[]) {
										{.type=IL_OP_NODE, .val.op=IL_DIE_OP, .id=19, .lineno=9, .num_children=1, .children=(struct il_node[]) {
											{.type=IL_VAR_NODE, .val.str="BUTTERFLY", .id=20, .lineno=9, .num_children=0, .children=NULL}}},
										{.type=IL_OP_NODE, .val.op=IL_IF_OP, .id=21, .lineno=10, .num_children=2, .children=(struct il_node[]) {
											{.type=IL_VAR_NODE, .val.str="LAMB", .id=22, .lineno=10, .num_children=0, .children=NULL},
											{.type=IL_OP_NODE, .val.op=IL_JMP_OP, .id=23, .lineno=10, .num_children=1, .children=(struct il_node[]) {
												{.type=IL_TAR_NODE, .val.tar=18, .id=24, .lineno=10, .num_children=0, .children=NULL}}}}}}}}},
								{.type=IL_OP_NODE, .val.op=IL_DIE_OP, .id=25, .lineno=12, .num_children=1, .children=(struct il_node[]) {
									{.type=IL_VAR_NODE, .val.str="THIS", .id=26, .lineno=12, .num_children=0, .children=NULL}}}}};

	if (validate_il(&input)) {
		fprintf(stderr, "IL tree invalidation test when attempting to kill an object overwritten to be a universe failed.\n");
		exit(EXIT_SUCCESS); 
	} else {
		fprintf(stderr, "IL tree invalidation test when attempting to kill an object overwritten to be a universe succeeded.\n");
		exit(EXIT_FAILURE);
	}
}

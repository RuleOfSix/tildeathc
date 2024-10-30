#ifndef TILDEATHC_IL_H
#define TILDEATHC_IL_H

#include <stdint.h>
struct ast;

enum il_operation {
	IL_IF_OP, 
	IL_JMP_OP,
	IL_DIE_OP,
	IL_IN_OP,
	IL_OUT_OP,
	IL_ABS_OP,
	IL_LIB_OP,
	IL_UNI_OP,
	IL_BIF_OP,
};

enum il_node_type {
	IL_VAR_NODE,
	IL_STR_NODE,
	IL_DEC_NODE,
	IL_TAR_NODE, 
	IL_OP_NODE,
	IL_ROOT_NODE,
};

union il_node_value {
	enum il_operation op;
	int64_t tar;
	char* str;
};

struct il_node {
	int64_t id;
	enum il_node_type type;
	union il_node_value val;
	int64_t lineno;
	int64_t num_children;
	struct il_node* children;
};

struct il_node* generate_il(const struct ast* ast);
void free_il_tree(struct il_node* il_tree);

#endif

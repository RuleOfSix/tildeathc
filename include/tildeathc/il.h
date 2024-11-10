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

#define LOOKUP_IL_OPERATION(il_operation) ((il_operation) == IL_IF_OP ? "IL_IF_OP" : (\
									(il_operation) == IL_JMP_OP ? "IL_JMP_OP" : (\
									(il_operation) == IL_DIE_OP ? "IL_DIE_OP" : (\
									(il_operation) == IL_IN_OP ? "IL_IN_OP" : (\
									(il_operation) == IL_OUT_OP ? "IL_OUT_OP" : (\
									(il_operation) == IL_ABS_OP ? "IL_ABS_OP" : (\
									(il_operation) == IL_LIB_OP ? "IL_LIB_OP" : (\
									(il_operation) == IL_UNI_OP ? "IL_UNI_OP" : (\
									(il_operation) == IL_BIF_OP ? "IL_BIF_OP" : "NONE")))))))))



#define LOOKUP_IL_TYPE(il_node_type) ((il_node_type) == IL_VAR_NODE ? "IL_VAR_NODE" : (\
									(il_node_type) == IL_STR_NODE ? "IL_STR_NODE" : (\
									(il_node_type) == IL_DEC_NODE ? "IL_DEC_NODE" : (\
									(il_node_type) == IL_TAR_NODE ? "IL_TAR_NODE" : (\
									(il_node_type) == IL_OP_NODE ? "IL_OP_NODE" : (\
									(il_node_type) == IL_ROOT_NODE ? "IL_ROOT_NODE" : "NONE"))))))

struct il_node* generate_il(const struct ast* ast);
void free_il_tree(struct il_node* il_tree);

#endif

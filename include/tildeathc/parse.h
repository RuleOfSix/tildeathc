#ifndef TILDEATHC_PARSE_H
#define TILDEATHC_PARSE_H

#include <stdbool.h>
#include <stdint.h>
struct token_list;

enum ast_operation {
	LOOP_OP,
	BIFURCATE_OP,
	DIE_OP,
	IMPORT_OP,
	PRINT_OP,
	NULL_OP,
};

enum ast_node_type {
	OPERATION_NODE,
	STRING_NODE,
	ROOT_NODE,
};

union ast_node_value {
	enum ast_operation op;
	char* str;
};

struct ast {
	enum ast_node_type type;
	union ast_node_value val;
	int64_t lineno;
	int64_t num_children;
	struct ast* children;
};

#define LOOKUP_OPERATION(operation) ((operation) == LOOP_OP ? "LOOP_OP" : (\
									(operation) == BIFURCATE_OP ? "BIFURCATE_OP" : (\
									(operation) == DIE_OP ? "DIE_OP" : (\
									(operation) == IMPORT_OP ? "IMPORT_OP" : (\
									(operation) == PRINT_OP ? "PRINT_OP" : (\
									(operation) == NULL_OP ? "NULL_OP" : "NONE"))))))

#define LOOKUP_NODE_TYPE(node_type) ((node_type) == OPERATION_NODE ? "OPERATION_NODE" : (\
									(node_type) == STRING_NODE ? "STRING_NODE" : (\
									(node_type) == ROOT_NODE ? "ROOT_NODE" : "NONE")))

struct ast* parse(const struct token_list* tokens, bool is_library);
void free_ast(struct ast* tree);
#endif

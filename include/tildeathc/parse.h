#ifndef TILDEATHC_PARSE_H
#define TILDEATHC_PARSE_H

#include <stdint.h>
#include "tokenize.h"

typedef enum operation {
	LOOP_OP,
	BIFURCATE_OP,
	DIE_OP,
	IMPORT_OP,
	PRINT_OP,
} operation;

typedef enum node_type {
	OPERATION_NODE,
	STRING_NODE,
	ROOT_NODE,
} node_type;

typedef union node_value {
	operation op;
	char* str;
} node_value;

typedef struct ast {
	node_type type;
	node_value val;
	struct ast* children;
	int64_t num_children;
} ast;

ast* parse(const token_list* tokens);
void free_ast(ast* tree);
#endif

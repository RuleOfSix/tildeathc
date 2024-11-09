#include <nullcheck.h>
#include <parse.h>
#include <il.h>
#include <util.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

enum variable_type {
	ABSTRACT,
	INPUT,
	UNIVERSE,
};

void convert_op_node(const struct ast* node, struct il_node* output);
void convert_loop_node(const struct ast* node, struct il_node* output);
void convert_bifurcate_node(const struct ast* node, struct il_node* output);
void convert_die_node(const struct ast* node, struct il_node* output);
void convert_import_node(const struct ast* node, struct il_node* output);
void convert_print_node(const struct ast* node, struct il_node* output);
void convert_declaration_node(const struct ast* node, struct il_node* output);
void convert_var_node(const struct ast* node, struct il_node* output);
void convert_string_node(const struct ast* node, struct il_node* output);
int64_t generate_id(void);

struct il_node* generate_il(const struct ast* ast) {
	struct il_node* output = malloc(sizeof(*output));
	MALLOC_NULL_CHECK(output);	

	output->type = IL_ROOT_NODE;
	output->id = generate_id();
	output->val.str = NULL;
	output->lineno = 0;
	output->num_children = ast->num_children;
	output->children = malloc(output->num_children * sizeof(*(output->children)));
	MALLOC_NULL_CHECK(output->children);

	for (int64_t i = 0; i < ast->num_children; i++) {
		convert_op_node(ast->children + i, output->children + i);
	}

	return output;
}

void convert_op_node(const struct ast* node, struct il_node* output) {
	output->id = generate_id();
	output->lineno = node->lineno;
	switch (node->type) {
		case ROOT_NODE:
			fprintf(stderr, "Internal Compiler Error: root AST node erroneously passed to convert_op_node function. Terminating.\n");
			exit(EXIT_FAILURE);
			break;
		case STRING_NODE:
			fprintf(stderr, "Internal Compiler Error: string AST node at line %ld erroneously passed to convert_op_node function. Terminating.\n", node->lineno);
			exit(EXIT_FAILURE);
			break;
		case OPERATION_NODE:
			switch (node->val.op) {
				case LOOP_OP:
					convert_loop_node(node, output);
					break;
				case BIFURCATE_OP:
					convert_bifurcate_node(node, output);
					break;
				case DIE_OP:
					convert_die_node(node, output);
					break;
				case IMPORT_OP:
					convert_import_node(node, output);
					break;
				case PRINT_OP:
					convert_print_node(node, output);
					break;
				case NULL_OP:
					output->id = -1;
					break;
			}
			break;
	}
}

void convert_loop_node(const struct ast* node, struct il_node* output) {
	if (node->num_children < 2 || node->children == NULL) {
		fprintf(stderr, "Internal Compiler Error: malformed LOOP_OP node at line %ld passed to convert_node function. Terminating.\n", node->lineno);
		exit(EXIT_FAILURE);
	}
	output->type = IL_OP_NODE;
	output->val.op = IL_IF_OP;
	output->num_children = node->children[node->num_children - 1].val.op == NULL_OP ? node->num_children : node->num_children + 1;

	for (int64_t i = 0; i < node->num_children - 1; i++) {
		convert_op_node(node->children + i, output->children + i);
	}

	struct il_node* jmp_node = &(output->children[node->num_children - 1]);
	jmp_node->type = IL_OP_NODE;
	jmp_node->val.op = IL_JMP_OP;
	jmp_node->id = generate_id();
	jmp_node->lineno = node->children[node->num_children - 1].lineno;
	jmp_node->num_children = 1; 
	jmp_node->children = malloc(sizeof(*(output->children))); 
	MALLOC_NULL_CHECK(jmp_node->children);
	jmp_node->children[0].type = IL_TAR_NODE;
	jmp_node->children[0].val.tar = output->id;
	jmp_node->children[0].id = generate_id();
	jmp_node->children[0].lineno = jmp_node->lineno;
	jmp_node->children[0].num_children = 0;
	jmp_node->children[0].children = NULL;
	
	if (node->children[node->num_children - 1].val.op != NULL_OP) {
		convert_op_node(node->children + node->num_children - 1, output->children + output->num_children - 1);
	}
}

void convert_bifurcate_node(const struct ast* node, struct il_node* output) {
	if (node->num_children != 3 || node->children == NULL) {
		fprintf(stderr, "Internal Compiler Error: malformed BIFURCATE_OP node at line %ld passed to convert_node function. Terminating.\n", node->lineno);
		exit(EXIT_FAILURE);
	}
	output->type = IL_OP_NODE;
	output->val.op = IL_BIF_OP;
	output->num_children = 3;
	output->children = malloc(output->num_children * sizeof(*(output->children)));
	MALLOC_NULL_CHECK(output->children);
	convert_var_node(node->children, output->children);
	convert_declaration_node(node->children + 1, output->children + 1);
	convert_declaration_node(node->children + 1, output->children + 1);
}

int64_t generate_id(void) {
	static int64_t num_ids = 0;
	return num_ids++;
}

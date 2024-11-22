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
void convert_abstract_node(const struct ast* node, struct il_node* output);
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
					fprintf(stderr, "Internal Compiler Error: NULL_OP AST node at line %ld erroneously passed to convert_op_node function. Terminating.\n", node->lineno);
					exit(EXIT_FAILURE);
					break;
			}
			break;
	}
}

void convert_loop_node(const struct ast* node, struct il_node* output) {
	if (node->num_children < 2 || node->children == NULL) {
		fprintf(stderr, "Internal Compiler Error: malformed LOOP_OP node at line %ld passed to convert_loop_node function. Terminating.\n", node->lineno);
		exit(EXIT_FAILURE);
	}
	output->type = IL_OP_NODE;
	output->val.op = IL_IF_OP;
	output->num_children = 2;
	output->children = malloc(sizeof(*(output->children)) * output->num_children);
	MALLOC_NULL_CHECK(output->children);
	
	convert_var_node(node->children, output->children);

	struct il_node* block_node = &(output->children[1]);
	block_node->id = generate_id();
	block_node->lineno = node->children[0].lineno;
	block_node->type = IL_OP_NODE;
	block_node->val.op = IL_BLOCK_OP;
	block_node->num_children = node->children[node->num_children - 1].val.op == NULL_OP ? node->num_children - 1 : node->num_children;
	block_node->children = malloc(sizeof(*(block_node->children)) * block_node->num_children);
	MALLOC_NULL_CHECK(block_node->children);


	for (int64_t i = 0; i < node->num_children - 2; i++) {
		convert_op_node(node->children + i + 1, block_node->children + i);
	}

	struct il_node* if_node = &(block_node->children[node->num_children - 2]);
	if_node->type = IL_OP_NODE;
	if_node->val.op = IL_IF_OP;
	if_node->id = generate_id();
	if_node->lineno = node->children[node->num_children - 1].lineno;
	if_node->num_children = 2; 
	if_node->children = malloc(sizeof(*(if_node->children)) * if_node->num_children); 
	MALLOC_NULL_CHECK(if_node->children);

	convert_var_node(node->children, if_node->children);
	if_node->children[0].lineno = if_node->lineno;

	struct il_node* jmp_node = &(if_node->children[1]);
	jmp_node->type = IL_OP_NODE;
	jmp_node->val.op = IL_JMP_OP;
	jmp_node->id = generate_id();
	jmp_node->lineno = if_node->lineno;
	jmp_node->num_children = 1; 
	jmp_node->children = malloc(sizeof(*(jmp_node->children)) * jmp_node->num_children); 
	MALLOC_NULL_CHECK(jmp_node->children);

	struct il_node* tar_node = &(jmp_node->children[0]);
	tar_node->type = IL_TAR_NODE;
	tar_node->val.tar = block_node->id;
	tar_node->id = generate_id();
	tar_node->lineno = jmp_node->lineno;
	tar_node->num_children = 0;
	tar_node->children = NULL;
	
	if (node->children[node->num_children - 1].val.op != NULL_OP) {
		convert_op_node(node->children + node->num_children - 1, block_node->children + block_node->num_children - 1);
	}
}

void convert_bifurcate_node(const struct ast* node, struct il_node* output) {
	if (node->num_children != 3 || node->children == NULL) {
		fprintf(stderr, "Internal Compiler Error: malformed BIFURCATE_OP node at line %ld passed to convert_bifurcate_node function. Terminating.\n", node->lineno);
		exit(EXIT_FAILURE);
	}
	output->type = IL_OP_NODE;
	output->val.op = IL_BIF_OP;
	output->num_children = 3;
	output->children = malloc(output->num_children * sizeof(*(output->children)));
	MALLOC_NULL_CHECK(output->children);
	convert_var_node(node->children, output->children);
	convert_abstract_node(node->children + 1, output->children + 1);
	convert_abstract_node(node->children + 2, output->children + 2);
}

void convert_die_node(const struct ast* node, struct il_node* output) {
	if (node->num_children != 1 || node->children == NULL) {
		fprintf(stderr, "Internal Compiler Error: malformed DIE_OP node at line %ld passed to convert_die_node function. Terminating.\n", node->lineno);
		exit(EXIT_FAILURE);
	}
	output->type = IL_OP_NODE;
	output->val.op = IL_DIE_OP;
	output->num_children = 1;
	output->children = malloc(output->num_children * sizeof(*(output->children)));
	MALLOC_NULL_CHECK(output->children);
	convert_var_node(node->children, output->children);		
}

void convert_import_node(const struct ast* node, struct il_node* output) {
	if (node->num_children != 2 || node->children == NULL || node->children[0].type != STRING_NODE || node->children[0].val.str == NULL) {
		fprintf(stderr, "Internal Compiler Error: malformed IMPORT_OP node at line %ld passed to convert_import_node function. Terminating.\n", node->lineno);
		exit(EXIT_FAILURE);
	}
	output->type = IL_OP_NODE;
	if (strcmp(node->children[0].val.str, "abstract") == 0) {
		output->val.op = IL_ABS_OP;
	} else if (strcmp(node->children[0].val.str, "universe") == 0) {
		output->val.op = IL_UNI_OP;
	} else if (strcmp(node->children[0].val.str, "library") == 0) {
		output->val.op = IL_LIB_OP;
	} else if (strcmp(node->children[0].val.str, "input") == 0) {
		output->val.op = IL_IN_OP;
	}
	output->num_children = 1;
	output->children = malloc(output->num_children * sizeof(*(output->children)));
	MALLOC_NULL_CHECK(output->children);
	convert_declaration_node(node->children + 1, output->children);	
}

void convert_print_node(const struct ast* node, struct il_node* output) {
	if (node->num_children != 1 || node->children == NULL) {
		fprintf(stderr, "Internal Compiler Error: malformed PRINT_OP node at line %ld passed to convert_print_node function. Terminating.\n", node->lineno);
		exit(EXIT_FAILURE);
	}
	output->type = IL_OP_NODE;
	output->val.op = IL_OUT_OP;
	output->num_children = 1;
	output->children = malloc(output->num_children * sizeof(*(output->children)));
	MALLOC_NULL_CHECK(output->children);
	convert_string_node(node->children, output->children);
}

void convert_abstract_node(const struct ast* node, struct il_node* output) {
	if (node->type !=STRING_NODE || node->val.str == NULL) {
		fprintf(stderr, "Internal Compiler Error: malformed STRING_NODE node at line %ld passed to convert_abstract_node function. Terminating\n", node->lineno);
		exit(EXIT_FAILURE);
	}
	output->id = generate_id();
	output->lineno = node->lineno;
	output->type = IL_OP_NODE;
	output->val.op = IL_ABS_OP;
	output->num_children = 1;
	output->children = malloc(output->num_children * sizeof(*(output->children)));
	convert_declaration_node(node, output->children);
}

void convert_declaration_node(const struct ast* node, struct il_node* output) {
	if (node->type != STRING_NODE || node->val.str == NULL) {
		fprintf(stderr, "Internal Compiler Error: malformed STRING_NODE node at line %ld passed to convert_declaration_node function. Terminating.\n", node->lineno);
		exit(EXIT_FAILURE);
	}
	output->id = generate_id();
	output->lineno = node->lineno;
	output->type = IL_DEC_NODE;
	output->val.str = util_newstr(node->val.str);
	output->num_children = 0;
	output->children = NULL;
}

void convert_var_node(const struct ast* node, struct il_node* output) {
	if (node->type != STRING_NODE || node->val.str == NULL) {
		fprintf(stderr, "Internal Compiler Error: malformed STRING_NODE node at line %ld passed to convert_var_node function. Terminating.\n", node->lineno);
		exit(EXIT_FAILURE);
	}
	output->id = generate_id();
	output->lineno = node->lineno;
	output->type = IL_VAR_NODE;
	output->val.str = util_newstr(node->val.str);
	output->num_children = 0;
	output->children = NULL;
}

void convert_string_node(const struct ast* node, struct il_node* output) {
	if (node->type != STRING_NODE || node->val.str == NULL) {
		fprintf(stderr, "Internal Compiler Error: malformed STRING_NODE node at line %ld passed to convert_string_node function. Terminating.\n", node->lineno);
		exit(EXIT_FAILURE);
	}
	output->id = generate_id();
	output->lineno = node->lineno;
	output->type = IL_STR_NODE;
	output->val.str = util_newstr(node->val.str);
	output->num_children = 0;
	output->children = NULL;
}

int64_t generate_id(void) {
	static int64_t num_ids = 0;
	return num_ids++;
}

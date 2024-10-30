#include <nullcheck.h>
#include <parse.h>
#include <il.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

enum variable_type {
	ABSTRACT,
	INPUT,
	UNIVERSE,
};

struct variable {
	char* name;
};

int64_t generate_id(void);
void convert_node(const struct ast* node, struct il_node* output, enum ast_operation parent_op);

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
		convert_node(ast->children + i, output->children + i, NULL_OP);
	}

	return output;
}

void convert_node(const struct ast* node, struct il_node* output, enum ast_operation parent_op) {
	output->id = generate_id();
	output->lineno = node->lineno;
	switch (node->type) {
		case ROOT_NODE:
			fprintf(stderr, "Error: root AST node erroneously passed to convert_node function. Terminating.\n");
			exit(EXIT_FAILURE);
			break;
		case STRING_NODE:
			switch (parent_op) {
				case IMPORT_OP:
					output->type = IL_DEC_NODE;
					break;
				case DIE_OP: //fallthrough
				case LOOP_OP: //fallthrough
				case BIFURCATE_OP:
					output->type = IL_VAR_NODE;
					break;
				case PRINT_OP:
					output->type = IL_STR_NODE;
				case NULL_OP:
					fprintf(stderr, "Error: child of NULL_OP AST node exists. Terminating.\n");
					exit(EXIT_FAILURE);
			}
			int32_t outlen = strlen(node->val.str);
			output->val.str = malloc((outlen + 1) * sizeof(char));
			memcpy(output->val.str, node->val.str, outlen + 1);
			output->num_children = 0;
			output->children = NULL;
			break;
		case OPERATION_NODE:
			switch (node->val.op) {
				case LOOP_OP:
					output->type = IL_OP_NODE;
					output->val.op = IL_IF_OP;
					output->num_children = node->children[node->num_children - 1].val.op == NULL_OP ? node->num_children : node->num_children + 1;

					for (int64_t i = 0; i < node->num_children - 1; i++) {
						convert_node(node->children + i, output->children + i, LOOP_OP);
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
						convert_node(node->children + node->num_children - 1, output->children + output->num_children - 1, LOOP_OP);
					}
					break;
				case BIFURCATE_OP:
					output->type = IL_OP_NODE;
					output->val.op = IL_BIF_OP;
					output->num_children = 3;
					output->children = malloc(output->num_children * sizeof(*(output->children)));
					MALLOC_NULL_CHECK(output->children);
					convert_node(node->children, output->children, BIFURCATE_OP);
					/* TODO: other two declaration nodes once variable-checking system has been made */
					break;
				case DIE_OP:
					break;
				case IMPORT_OP:
					break;
				case PRINT_OP:
					break;
				case NULL_OP:
					output->id = -1;
					break;
			}
			break;
	}
}

int64_t generate_id(void) {
	static int64_t num_ids = 0;
	return num_ids++;
}

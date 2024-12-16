#include<project_info.h>
#include<tokenize.h>
#include<parse.h>
#include<il.h>
#include<validate_il.h>
#include<stdio.h>
#include<stdint.h>
#include<stdbool.h>
#include<stdlib.h>
#include<errno.h>

void print_il_tree_definition(struct il_node* il_tree, int num_indents, bool final_child);

/* 
 * This script is designed to make designing codegen tests easier and 
 * less error-prone by printing out an il_tree definition from a ~ATH 
 * source file. Simply pass in the filename of the ~ATH source as the 
 * script's only argument. The output should always be reviewed before
 * inclusion in a test, and the original ~ATH file should not be kept
 * in the repository.  
 */
int32_t main(int argc, char* argv[]) {
	if (argc > 2) {
		fprintf(stderr, "Usage: generate_codegen_test_input path_to_source_file\n");
		exit(EXIT_FAILURE);
	}
	errno = 0;
	FILE* input = fopen(argv[1], "r");
	if (input == NULL) {
		perror("Error reading input file");
		exit(EXIT_FAILURE);
	}

	struct token_list* tokens = tokenize(input);
	fclose(input);
	struct ast* syntax_tree = parse(tokens, false);
	free_token_list(tokens);
	free(tokens);
	struct il_node* il_tree = generate_il(syntax_tree, false, NULL);
	free_ast(syntax_tree);
	free(syntax_tree);
	if (!validate_il(il_tree)) {
		exit(EXIT_FAILURE);
	}

	print_il_tree_definition(il_tree, 0, false);
}

void print_il_tree_definition(struct il_node* node, int num_indents, bool final_child) {
	for (int32_t i = 0; i < num_indents; i++) {
		printf("\t");
	}

	printf("{.type=%s, ", LOOKUP_IL_TYPE(node->type));
	switch(node->type) {
		case IL_ROOT_NODE:
			printf(".val.str=NULL, ");
			break;
		case IL_OP_NODE:
			printf(".val.op=%s, ", LOOKUP_IL_OPERATION(node->val.op));
			break;
		case IL_VAR_NODE: //fallthrough
		case IL_STR_NODE: //fallthrough
		case IL_DEC_NODE: 
			printf(".val.str=\"%s\", ", node->val.str);
			break;
		case IL_TAR_NODE:
			printf(".val.tar=%ld, ", node->val.tar);
			break;
	}
	printf(".id=%ld, .lineno=%ld, .num_children=%ld, ", node->id, node->lineno, node->num_children);
	if (node->children == NULL) {
		printf(".children=NULL");
	} else {
		printf(".children=(struct il_node[]) {\n");
		for (int64_t i = 0; i < node->num_children; i++) {
			print_il_tree_definition(node->children + i, num_indents + 1, i == node->num_children - 1);
		}
		printf("}");
	}
	if (node->type == IL_ROOT_NODE) {
		printf("};\n");
	} else if (final_child) {
		printf("}");
	} else {
		printf("},\n");
	}
}

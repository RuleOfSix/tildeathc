#include <parse.h>
#include <project_info.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

bool compare(ast* expected, ast* actual);

int32_t main() {
	token input_tokens[] = {
		{"~ATH", 1},
		{"(", 1},
		{"LAMB", 1},
		{")", 1},
		{"{", 1},
		{"}", 1},
		{"EXECUTE", 1},
		{"(", 1},
		{"NULL", 1},
		{")", 1},
		{";", 1},
		{"THIS", 2},
		{".DIE", 2},
		{"(", 2},
		{")", 2},
		{";", 2}
	};
	token_list input = {input_tokens, 16};

	ast loop_c[] = {{.type=STRING_NODE, .val.str="LAMB", .lineno=1, .num_children=0, .children=NULL},
						  {.type=OPERATION_NODE, .val.op=NULL_OP, .lineno=1, .num_children=0, .children=NULL}};
	ast death_c[] = {{.type=STRING_NODE, .val.str="THIS", .lineno=2, .num_children=0, .children=NULL}};
	ast root_c[] = {{.type=OPERATION_NODE, .val.op=LOOP_OP, .lineno=1, .num_children=2, .children=loop_c},
						{.type=OPERATION_NODE, .val.op=DIE_OP, .lineno=2, .num_children=1, .children=death_c}};
	ast expected_output = {.type=ROOT_NODE, .val.str=NULL, .lineno=0, .num_children=2, .children=root_c};

	ast* output = parse(&input);
	if (output == NULL) {
		fprintf(stderr, "Test failed: parse output null.\n");
		exit(EXIT_FAILURE);
	}
	if (!compare(&expected_output, output)) {
		fprintf(stderr, "Test failed: parse output does not match expected AST.\n");
		exit(EXIT_FAILURE);
	}
	printf("Parse module empty loop test passed\n");
	free_ast(output);
	free(output);
	return 0;
}

bool compare(ast* expected, ast* actual) {
	if (expected->type != actual->type) {
		fprintf(stderr, "Expected node type of %s, got %s on line %ld.\n", LOOKUP_NODE_TYPE(expected->type), LOOKUP_NODE_TYPE(actual->type), actual->lineno);
		return false;
	}
	if (expected->lineno != actual->lineno) {
		fprintf(stderr, "Expected line number of %ld, got %ld for node of type %s.\n", expected->lineno, actual->lineno, LOOKUP_NODE_TYPE(actual->type));
		return false;
	}
	if (expected->type == STRING_NODE) {
		if (strcmp(expected->val.str, actual->val.str) != 0) {
			fprintf(stderr, "Expected string value of %s, got %s on line %ld.\n", expected->val.str, actual->val.str, actual->lineno);
			return false;
		}
	}
	if (expected->type == OPERATION_NODE) {
		if (expected->val.op != actual->val.op) {
			fprintf(stderr, "Expected operation value of %s, got %s on line %ld.\n", LOOKUP_OPERATION(expected->val.op), LOOKUP_OPERATION(actual->val.op), actual->lineno);
			return false;
		}
	}
	if (expected->num_children != actual->num_children) {
		fprintf(stderr, "Expected %ld children, got %ld on line %ld.\n", expected->num_children, actual->num_children, actual->lineno);
		return false;
	}
	for (int64_t i = 0; i < expected->num_children; i++) {
		if (!compare(&(expected->children[i]), &(actual->children[i]))) {
			return false;	
		}
	}
	return true;
}

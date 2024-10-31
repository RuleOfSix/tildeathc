#include <tildeathc_test.h>
#include <nullcheck.h>
#include <tokenize.h>
#include <parse.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>



void tokenize_test(FILE* input, struct token_list* expected, bool should_fail) {
	ASSERT_NOT_NULL(input);	
	if (!should_fail) {
		ASSERT_NOT_NULL(expected);	
	}
	struct token_list* output = tokenize(input);
	if (should_fail) {
		fprintf(stderr, "Tokenize test failed: tokenize did not fail with incorrect input.\n");
		exit(EXIT_SUCCESS);
	}
	if (output == NULL) {
		fprintf(stderr, "Tokenize test failed: tokenize output null.\n");
		exit(EXIT_FAILURE);
	}
	if (!compare_token_list(expected, output)) {
		fprintf(stderr, "Tokenize test failed: output token list does not match what was expected.\n");
		exit(EXIT_FAILURE);
	}
}

void parse_test(struct token_list* input, struct ast* expected, bool should_fail) {
	ASSERT_NOT_NULL(input);
	if (!should_fail) {
		ASSERT_NOT_NULL(expected);
	}
	struct ast* output = parse(input);
	if (should_fail) {
		fprintf(stderr, "Parse test failed: parse did not fail with incorrect input.\n");
		exit(EXIT_SUCCESS);
	}
	if (output == NULL) {
		fprintf(stderr, "Parse test failed: parse output null.\n");
		exit(EXIT_FAILURE);	
	}	
	if (!compare_ast(expected, output)) {
		fprintf(stderr, "Parse test failed: output AST does not match waht was expected.\n");
		exit(EXIT_FAILURE);
	}
}

bool compare_ast(struct ast* expected, struct ast* actual) {
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
		if (!compare_ast(&(expected->children[i]), &(actual->children[i]))) {
			return false;	
		}
	}
	return true;
}

bool compare_token_list(struct token_list* expected, struct token_list* actual) {
	if (actual->length != expected->length) {
		fprintf(stderr, "Test failed: expected length property of %ld, output was %ld.\n", expected->length, actual->length);
		return false;
	}

	for (int64_t i = 0; i < actual->length; i++) {
		if (strcmp(actual->tokens[i].str, expected->tokens[i].str) != 0) {
			fprintf(stderr, "Test failed: expected token %ld's str to be %s, output was %s.\n", i, expected->tokens[i].str, actual->tokens[i].str);
			return false;
		}
		if (actual->tokens[i].lineno != expected->tokens[i].lineno) {
			fprintf(stderr, "Test failed: expected token %ld's lineno to be %ld, output was %ld.\n", i, expected->tokens[i].lineno, actual->tokens[i].lineno);
			return false;
		}
	}
	return true;
}

#include <tokenize.h>
#include <nullcheck.h>
#include <parse.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <string.h>

int64_t grave(ast* parent, const token_list* tokens, int64_t start_token);
int64_t loop(ast* parent, const token_list* tokens, int64_t start_token);
int64_t bifurcate(ast* parent, const token_list* tokens, int64_t start_token);
int64_t death(ast* parent, const token_list* tokens, int64_t start_token);
int64_t import(ast* parent, const token_list* tokens, int64_t start_token);
int64_t execute(ast* parent, const token_list* tokens, int64_t start_token);
int64_t is_valid_var(const char* str);

// THE FOLLOWING MACROS EVALUATE ARGUMENTS MULTIPLE TIMES.
#define ASSERT_TOKEN_MATCH(token, assertion, lineno) do { \
	if (strcmp((token), (assertion)) != 0) { \
		fprintf(stderr, "Syntax error on line %ld: expected '%s', got '%s' instead.\n", (lineno), (assertion), (token)); \
		exit(EXIT_FAILURE); \
	} \
} while (0)

#define ASSERT_VARNAME(str, lineno) do { \
	if (!is_valid_var(str)) { \
		fprintf(stderr, "Syntax error on line %ld: '%s' is an invalid variable name.\n", (lineno), (str)); \
		exit(EXIT_FAILURE); \
	} \
} while (0)

#define ASSERT_TOKEN_IN_BOUNDS(cur_token, token_list) do { \
	if (cur_token > token_list->length) { \
		fprintf(stderr, "Error: unexpectedly encountered end of file. Do you have an incomplete statement?\n"); \
	} \
} while (0)

ast* parse(const token_list* tokens) {
	ast* root = malloc(sizeof(*root));
	MALLOC_NULL_CHECK(root);
	root->type = ROOT_NODE;
	root->val.str = NULL;
	root->num_children = 0;
	root->children = NULL;
	for (int64_t i = 0; i < tokens->length;) {
		i += grave(root, tokens, i);
	}
	ast* last = &(root->children[root->num_children - 1]);
	if (last->type != OPERATION_NODE || last->val.op != DIE_OP || strcmp(last->children[0].val.str, "THIS") != 0) {
		fprintf(stderr, "Error: final grave is not THIS.DIE()\n");
		exit(EXIT_FAILURE);
	}

	return root;
}

int64_t grave(ast* parent, const token_list* tokens, int64_t start_token) {
	int64_t cur_token = start_token;
	if (strcmp(tokens->tokens[start_token].str, "~ATH") == 0) {
		cur_token += loop(parent, tokens, start_token);
		return cur_token;
	}
	if (strcmp(tokens->tokens[start_token].str, "bifurcate") == 0) {
		cur_token += bifurcate(parent, tokens, start_token);
		return cur_token;
	}
	if (strcmp(tokens->tokens[start_token].str, "import") == 0) {
		cur_token += import(parent, tokens, start_token);
		return cur_token;
	}
	cur_token += death(parent, tokens, start_token);
	return cur_token - start_token;
}

int64_t loop(ast* parent, const token_list* tokens, int64_t start_token) {
	ASSERT_NOT_NULL(parent);
	parent->num_children++;
	if (parent->num_children == 1) {
		parent->children = malloc(sizeof(*(parent->children)) * parent->num_children);	
	} else {
		parent->children = realloc(parent->children, sizeof(*(parent->children)) * parent->num_children);
	}
	MALLOC_NULL_CHECK(parent->children);
	ast* this_node = &(parent->children[parent->num_children - 1]);
	this_node->type = OPERATION_NODE;
	this_node->val.op = LOOP_OP;
	this_node->num_children = 1;
	this_node->children = malloc(sizeof(*(this_node->children)));
	MALLOC_NULL_CHECK(this_node->children);

	int64_t cur_token = start_token + 1;

	ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, "(", tokens->tokens[cur_token].lineno);
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	{
		ASSERT_VARNAME(tokens->tokens[cur_token].str, tokens->tokens[cur_token].lineno);
		this_node->children[0].type = STRING_NODE;
		int32_t varlen = strlen(tokens->tokens[cur_token].str);
		this_node->children[0].val.str = malloc((varlen + 1) * sizeof(char));
		MALLOC_NULL_CHECK(this_node->children[0].val.str);
		memcpy(this_node->children[0].val.str, tokens->tokens[cur_token].str, varlen + 1);
		this_node->children[0].num_children = 0;
		this_node->children[0].children = NULL;
		cur_token++;
		ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);
	}

	ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, ")", tokens->tokens[cur_token].lineno);
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, "{", tokens->tokens[cur_token].lineno);
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	while(strcmp(tokens->tokens[cur_token].str, "}") != 0) {
		cur_token += grave(this_node, tokens, cur_token);
		ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);
	}
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	cur_token += execute(this_node, tokens, cur_token);

	return cur_token - start_token;
}

int64_t bifurcate(ast* parent, const token_list* tokens, int64_t start_token) {
	ASSERT_NOT_NULL(parent);
	parent->num_children++;
	if (parent->num_children == 1) {
		parent->children = malloc(sizeof(*(parent->children)) * parent->num_children);	
	} else {
		parent->children = realloc(parent->children, sizeof(*(parent->children)) * parent->num_children);
	}
	MALLOC_NULL_CHECK(parent->children);
	ast* this_node = &(parent->children[parent->num_children - 1]);
	this_node->type = OPERATION_NODE;
	this_node->val.op = BIFURCATE_OP;
	this_node->num_children = 3;
	this_node->children = malloc(3 * sizeof(*(this_node->children)));

	int64_t cur_token = start_token + 1;

	{
		ASSERT_VARNAME(tokens->tokens[cur_token].str, tokens->tokens[cur_token].lineno);
		this_node->children[0].type = STRING_NODE;
		int32_t varlen = strlen(tokens->tokens[cur_token].str);
		this_node->children[0].val.str = malloc((varlen + 1) * sizeof(char));
		MALLOC_NULL_CHECK(this_node->children[0].val.str);
		memcpy(this_node->children[0].val.str, tokens->tokens[cur_token].str, varlen + 1);
		this_node->children[0].num_children = 0;
		this_node->children[0].children = NULL;
		cur_token++;
		ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);
	}

	ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, "[", tokens->tokens[cur_token].lineno);
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);
	
	{
		ASSERT_VARNAME(tokens->tokens[cur_token].str, tokens->tokens[cur_token].lineno);
		this_node->children[1].type = STRING_NODE;
		int32_t varlen = strlen(tokens->tokens[cur_token].str);
		this_node->children[1].val.str = malloc((varlen + 1) * sizeof(char));
		MALLOC_NULL_CHECK(this_node->children[1].val.str);
		memcpy(this_node->children[1].val.str, tokens->tokens[cur_token].str, varlen + 1);
		this_node->children[1].num_children = 0;
		this_node->children[1].children = NULL;
		cur_token++;
		ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);
	}

	ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, ",", tokens->tokens[cur_token].lineno);
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	{
		ASSERT_VARNAME(tokens->tokens[cur_token].str, tokens->tokens[cur_token].lineno);
		this_node->children[2].type = STRING_NODE;
		int32_t varlen = strlen(tokens->tokens[cur_token].str);
		this_node->children[2].val.str = malloc((varlen + 1) * sizeof(char));
		MALLOC_NULL_CHECK(this_node->children[2].val.str);
		memcpy(this_node->children[2].val.str, tokens->tokens[cur_token].str, varlen + 1);
		this_node->children[2].num_children = 0;
		this_node->children[2].children = NULL;
		cur_token++;
		ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);
	}

	ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, "]", tokens->tokens[cur_token].lineno);
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, ";", tokens->tokens[cur_token].lineno);
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	return cur_token - start_token;
}

void free_ast(ast *tree) {
	if (tree->num_children > 0) {
		for (int64_t i = 0; i < tree->num_children; i++) {
			free_ast(&(tree->children[i]));
		}
	}
	if (tree->type == STRING_NODE) {
		free(tree->val.str);
	}
	free(tree);
}
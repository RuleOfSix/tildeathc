#include <tokenize.h>
#include <nullcheck.h>
#include <parse.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>

int64_t grave(ast* parent, const token_list* tokens, int64_t start_token);
int64_t loop(ast* parent, const token_list* tokens, int64_t start_token);
int64_t bifurcate(ast* parent, const token_list* tokens, int64_t start_token);
int64_t death(ast* parent, const token_list* tokens, int64_t start_token);
int64_t import(ast* parent, const token_list* tokens, int64_t start_token);
int64_t execute(ast* parent, const token_list* tokens, int64_t start_token);
int64_t print(ast* parent, const token_list* tokens, int64_t start_token);
int64_t variable(ast* parent, const token_list* tokens, int64_t start_token);
bool is_valid_var(const char* str);

// THE FOLLOWING MACROS EVALUATE ARGUMENTS MULTIPLE TIMES.
#define ASSERT_TOKEN_MATCH(token, assertion, lineno) do { \
	if (strcmp((token), (assertion)) != 0) { \
		fprintf(stderr, "Syntax error on line %ld: expected '%s', got '%s' instead.\n", (lineno), (assertion), (token)); \
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
		return cur_token - start_token;
	}
	if (strcmp(tokens->tokens[start_token].str, "bifurcate") == 0) {
		cur_token += bifurcate(parent, tokens, start_token);
		return cur_token - start_token;
	}
	if (strcmp(tokens->tokens[start_token].str, "import") == 0) {
		cur_token += import(parent, tokens, start_token);
		return cur_token - start_token;
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
	this_node->num_children = 0;
	this_node->children = NULL;

	int64_t cur_token = start_token + 1;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, "(", tokens->tokens[cur_token].lineno);
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	cur_token += variable(this_node, tokens, cur_token);	

	ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, ")", tokens->tokens[cur_token].lineno);
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, "{", tokens->tokens[cur_token].lineno);
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	while(strcmp(tokens->tokens[cur_token].str, "}") != 0) {
		cur_token += grave(this_node, tokens, cur_token);
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
	this_node->num_children = 0;
	this_node->children = NULL;

	int64_t cur_token = start_token + 1;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	cur_token += variable(this_node, tokens, cur_token);	

	ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, "[", tokens->tokens[cur_token].lineno);
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);
	
	cur_token += variable(this_node, tokens, cur_token);	

	ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, ",", tokens->tokens[cur_token].lineno);
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	cur_token += variable(this_node, tokens, cur_token);	

	ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, "]", tokens->tokens[cur_token].lineno);
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, ";", tokens->tokens[cur_token].lineno);
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	return cur_token - start_token;
}

int64_t death(ast* parent, const token_list* tokens, int64_t start_token) {
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
	this_node->val.op = DIE_OP;
	this_node->num_children = 0;
	this_node->children = NULL;

	int64_t cur_token = start_token;

	if (strcmp(tokens->tokens[cur_token].str, "[") == 0) {
		cur_token++;
		ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

		cur_token += variable(this_node, tokens, cur_token);

		while (strcmp(tokens->tokens[cur_token].str, ",") == 0) {
			cur_token++;
			ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);
			
			cur_token += variable(this_node, tokens, cur_token);
		}

		ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, "]", tokens->tokens[cur_token].lineno);
		cur_token++;
		ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);
	} else {
		cur_token += variable(this_node, tokens, cur_token);
	}
	
	ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, ".DIE", tokens->tokens[cur_token].lineno);
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, "(", tokens->tokens[cur_token].lineno);
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, ")", tokens->tokens[cur_token].lineno);
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, ";", tokens->tokens[cur_token].lineno);
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	return cur_token - start_token;
}

int64_t import(ast* parent, const token_list* tokens, int64_t start_token) {
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
	this_node->val.op = IMPORT_OP;
	this_node->num_children = 1;
	this_node->children = malloc(this_node->num_children * sizeof(*(this_node->children)));
	MALLOC_NULL_CHECK(this_node->children);

	int64_t cur_token = start_token + 1;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);	

	const char* valid_import_names[] = {"abstract", "universe", "input", "library"};
	const int32_t num_import_names = 4;
	bool is_valid_import = false;
	for (int32_t i = 0; i < num_import_names; i++) {
		if (strcmp(valid_import_names[i], tokens->tokens[cur_token].str) == 0) {
			is_valid_import = true;
			break;
		}
	}
	if (!is_valid_import) {
		fprintf(stderr, "Error: invalid import type '%s' at line %ld.\n", tokens->tokens[cur_token].str, tokens->tokens[cur_token].lineno);
		exit(EXIT_FAILURE);
	}
	this_node->children[0].type = STRING_NODE;
	int32_t implen = strlen(tokens->tokens[cur_token].str);
	this_node->children[0].val.str = malloc((implen + 1) * sizeof(char));
	MALLOC_NULL_CHECK(this_node->children[0].val.str);
	memcpy(this_node->children[0].val.str, tokens->tokens[cur_token].str, implen + 1);
	this_node->children[0].num_children = 0;
	this_node->children[0].children = NULL;
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	cur_token += variable(this_node, tokens, cur_token);

	ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, ";", tokens->tokens[cur_token].lineno);
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);
	
	return cur_token - start_token;
}

int64_t execute(ast* parent, const token_list* tokens, int64_t start_token) {
	int64_t cur_token = start_token;

	ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, "EXECUTE", tokens->tokens[cur_token].lineno);
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, "(", tokens->tokens[cur_token].lineno);
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	if (strcmp(tokens->tokens[cur_token].str, "PRINT") == 0) {
		cur_token += print(parent, tokens, cur_token);
	} else {
		cur_token += grave(parent, tokens, cur_token);
	}
	
	ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, ")", tokens->tokens[cur_token].lineno);
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, ";", tokens->tokens[cur_token].lineno);
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	return cur_token - start_token;
}

int64_t print(ast* parent, const token_list* tokens, int64_t start_token) {
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
	this_node->val.op = PRINT_OP;
	this_node->num_children = 1;
	this_node->children = malloc(sizeof(*(this_node->children)));
	MALLOC_NULL_CHECK(this_node->children);

	int64_t cur_token = start_token + 1;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);
	
	if (tokens->tokens[cur_token].str[0] != '"') {
		fprintf(stderr, "Error: PRINT statement on line %ld not followed by a string.\n", tokens->tokens[cur_token].lineno);
		exit(EXIT_FAILURE);
	}

	this_node->children[0].type = STRING_NODE;
	int32_t printlen = strlen(tokens->tokens[cur_token].str) - 2;
	this_node->children[0].val.str = malloc((printlen + 1) * sizeof(char));
	MALLOC_NULL_CHECK(this_node->children[0].val.str);
	memcpy(this_node->children[0].val.str, tokens->tokens[cur_token].str + 1, printlen);
	this_node->children[0].val.str[printlen] = '\0';
	this_node->children[0].num_children = 0;
	this_node->children[0].children = NULL;
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	ASSERT_TOKEN_MATCH(tokens->tokens[cur_token].str, ";", tokens->tokens[cur_token].lineno);
	cur_token++;
	ASSERT_TOKEN_IN_BOUNDS(cur_token, tokens);

	return cur_token - start_token;
}

int64_t variable(ast* parent, const token_list* tokens, int64_t start_token) {
	if (!is_valid_var(tokens->tokens[start_token].str)) { 
		fprintf(stderr, "Syntax error on line %ld: '%s' is an invalid variable name.\n", tokens->tokens[start_token].lineno, tokens->tokens[start_token].str);
		exit(EXIT_FAILURE);
	}

	ASSERT_NOT_NULL(parent);
	parent->num_children++;
	if (parent->num_children == 1) {
		parent->children = malloc(sizeof(*(parent->children)) * parent->num_children);	
	} else {
		parent->children = realloc(parent->children, sizeof(*(parent->children)) * parent->num_children);
	}
	MALLOC_NULL_CHECK(parent->children);
	ast* this_node = &(parent->children[parent->num_children - 1]);
	this_node->type = STRING_NODE;
	this_node->children = NULL;
	this_node->num_children = 0;

	int32_t varlen = strlen(tokens->tokens[start_token].str);
	this_node->val.str = malloc((varlen + 1) * sizeof(char));
	MALLOC_NULL_CHECK(this_node->val.str);
	memcpy(this_node->val.str, tokens->tokens[start_token].str, varlen + 1);

	ASSERT_TOKEN_IN_BOUNDS(start_token + 1, tokens);
	return 1;
}

bool is_valid_var(const char* str) {
	if (*str == '\0') {
		return false;
	}
	do {
		if (!(isupper(*str) || isdigit(*str) || *str == '_')) {
			return false;
		}
	} while (*(++str) != '\0');
	return true;
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

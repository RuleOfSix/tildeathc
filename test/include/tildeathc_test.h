#ifndef TILDEATHC_TEST_H
#define TILDEATHC_TEST_H

#include <stdio.h>
#include <stdbool.h>

struct token_list;
struct ast;
struct il_node;

bool compare_ast(const struct ast* expected, const struct ast* actual);
bool compare_token_list(const struct token_list* expected, const struct token_list* actual);
bool compare_il_tree(const struct il_node* expected, const struct il_node* actual);

void tokenize_test(FILE* input, const struct token_list* expected, bool should_fail);
void parse_test(const struct token_list* input, const struct ast* expected, bool should_fail);
void il_test(const struct ast* input, const struct il_node* expected, bool should_fail);
void codegen_test(const struct il_node* input, char* expected, bool should_fail);

#endif

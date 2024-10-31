#ifndef TILDEATHC_TEST_H
#define TILDEATHC_TEST_H

#include <stdio.h>
#include <stdbool.h>

struct token_list;
struct ast;
struct il_node;

bool compare_ast(struct ast* expected, struct ast* actual);
bool compare_token_list(struct token_list* expected, struct token_list* actual);

void tokenize_test(FILE* input, struct token_list* expected, bool should_fail);
void parse_test(struct token_list* input, struct ast* expected, bool should_fail);
void il_test(struct ast* input, struct il_node* expected, bool should_fail);

#endif

#ifndef TILDEATH_TOKENIZE_H
#define TILDEATH_TOKENIZE_H

#include <stdio.h>
#include <stdint.h>

typedef struct token {
	char* str;
	int64_t lineno;
} token;

typedef struct token_list {
	token* tokens;
	int64_t length;
} token_list;

token_list* tokenize(FILE* stream);
void free_token_list(token_list* tl);

#endif

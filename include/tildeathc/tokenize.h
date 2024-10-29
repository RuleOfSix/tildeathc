#ifndef TILDEATHC_TOKENIZE_H
#define TILDEATHC_TOKENIZE_H

#include <stdio.h>
#include <stdint.h>

struct token {
	char* str;
	int64_t lineno;
};

struct token_list {
	struct token* tokens;
	int64_t length;
};

struct token_list* tokenize(FILE* stream);
void free_token_list(struct token_list* tl);

#endif

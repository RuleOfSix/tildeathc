#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include <tokenize.h>
#include <nullcheck.h>

#define NUM_TOKENS 15

token* get_next_token(char** instr, int64_t lineno, const char** token_array, int16_t num_tokens);
bool is_whitespace(char* str);

token_list* tokenize(FILE* stream) {
	const char* tokens[] = {"~ATH", "EXECUTE", "bifurcate", ".DIE", "PRINT", "import", "(", ")", "[", "]", "{", "}", ";", ",", "\""};

	size_t size = 0;
	char* line = NULL;

	int32_t n = 50;
	token_list* output = malloc(sizeof(*output));
	MALLOC_NULL_CHECK(output);
	output->tokens = malloc(n * sizeof(*output->tokens));
	MALLOC_NULL_CHECK(output->tokens);
	output->length = 0;
	
	for (int64_t lineno = 1; getline(&line, &size, stream) > 0; lineno++) {
		token* next_token;
		char* p = line;
		while((next_token = get_next_token(&p, lineno, tokens, NUM_TOKENS)) != NULL)	{
			output->length++;
			if (output->length > n) {
				n *= 2;
				output->tokens = realloc(output->tokens, n * sizeof(*output->tokens));
				MALLOC_NULL_CHECK(output->tokens);
			}
			output->tokens[output->length - 1] = *next_token;
			free(next_token);
		}
	}
	free(line);
	return output;
}

token* get_next_token(char** instr, int64_t lineno, const char** const token_array, int16_t num_tokens) {
	token* output = NULL;

	if (is_whitespace(*instr)) {
		goto failed_to_find;
	}

	while(isspace(**instr)) {
		(*instr)++;
	}

	for (int16_t i = 0; i < num_tokens; i++) {
		if (strstr(*instr,token_array[i]) != *instr) {
			continue;
		}
		int16_t token_length = strlen(token_array[i]);
		output = malloc(sizeof(*output));
		MALLOC_NULL_CHECK(output);
		output->str = malloc((token_length + 1) * sizeof(*output->str));
		MALLOC_NULL_CHECK(output->str);
		memcpy(output->str, token_array[i], token_length + 1);

		for (int16_t j = 0; j < token_length; j++) {
			(*instr)++;
		}
		break;
	}

	if (output == NULL) {
		int16_t i = 0;
		char* q;
		for (q = *instr; !isspace(*q) && (strchr(".\";(){}[]", *q) == NULL); q++) {
			if (!isprint(*q)) {
				fprintf(stderr, "Input stream contains malformed character data. Terminating.\n");
				exit(EXIT_FAILURE);
			}
			if (*q == '\0')	{
				goto failed_to_find;
			}
			i++;
		}
		output = malloc(sizeof(*output));
		MALLOC_NULL_CHECK(output);
		output->str = malloc((i + 1) * sizeof(*output->str));
		MALLOC_NULL_CHECK(output->str);
		memcpy(output->str, *instr, i);
		output->str[i] = '\0';
		*instr = q;
	}

	if (output != NULL) {
		output->lineno = lineno;
	}

	failed_to_find:;
	return output;
}

void free_token_list(token_list* tl) {
	for (int64_t i = 0; i < tl->length; i++) {
		free(tl->tokens[i].str);
	}
	free(tl->tokens);
}

bool is_whitespace(char* str) {
	while (isspace(*str)) {
		str++;
	}
	if (*str == '\0') {
		return true;
	}
	return false;
}

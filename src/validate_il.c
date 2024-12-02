#include <validate_il.h>
#include <nullcheck.h>
#include <util.h>
#include <il.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

bool in_varlist(char* str, struct varlist* vars);
void append_to_varlist(char* str, struct varlist* vars);

bool validate_il(struct il_node* node, struct varlist* vars) {
	bool should_free = false;
	if (vars == NULL) {
		should_free = true;
		vars = malloc(sizeof(*vars));
		MALLOC_NULL_CHECK(vars);
		vars->memsize = 10;
		vars->length = 0;
		vars->list = malloc(vars->memsize * sizeof(*(vars->list)));
	}

	if (node->type == IL_DEC_NODE && !in_varlist(node->val.str, vars)) {
		append_to_varlist(node->val.str, vars);
		return true;
	}
	if (node->type == IL_VAR_NODE || node->type == IL_STR_NODE) {
		return in_varlist(node->val.str, vars);
	}

	bool is_valid = true;
	for (int64_t i = 0; i < node->num_children; i++) {
		 if (!validate_il(node->children + i, vars)) {
			 is_valid = false;
		 }
	}
	
	if (should_free) {
		free_varlist(vars);
		free(vars);
	}

	return is_valid;
}

bool in_varlist(char* str, struct varlist* vars) {
	bool in_varlist = false;
	for (int64_t i = 0; i < vars->length; i++) {
		if (strcmp(str, vars->list[i]) == 0) {
			in_varlist = true;
			break;
		}
	}
	return in_varlist;
}

void append_to_varlist(char* str, struct varlist* vars) {
	if (vars->length >= vars->memsize) {
		vars->memsize *= 2;
		vars->list = realloc(vars->list, vars->memsize);
		MALLOC_NULL_CHECK(vars);
	}
	
	vars->list[vars->length] = util_strdup(str);
	vars->length++;
	return;
}

void free_varlist(struct varlist* vars) {
	for (int64_t i = 0; i < vars->length; i++) {
		free(vars->list[i]);
	}
}

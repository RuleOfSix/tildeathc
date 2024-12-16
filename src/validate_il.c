#include <validate_il.h>
#include <nullcheck.h>
#include <util.h>
#include <il.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum tildeath_object_type {
	ABSTRACT,
	UNIVERSE,
	INPUT
};

struct varlist {
	int64_t memsize;
	int64_t length;
	char** list;
	enum tildeath_object_type* type_list;
};

bool validate_il_node(struct il_node* node, struct varlist* vars);
int64_t varlist_index (char* str, struct varlist* vars);
void append_to_varlist(char* str, enum tildeath_object_type op, struct varlist* vars);
enum tildeath_object_type op_to_var_type(enum il_operation op);


bool validate_il(struct il_node* node) {
	return validate_il_node(node, NULL); 
}

bool validate_il_node(struct il_node* node, struct varlist* vars) {
	bool is_valid = true;
	bool should_free = false;
	if (vars == NULL) {
		should_free = true;
		vars = malloc(sizeof(*vars));
		MALLOC_NULL_CHECK(vars);
		vars->memsize = 10;
		vars->length = 0;
		vars->list = malloc(vars->memsize * sizeof(*(vars->list)));
		MALLOC_NULL_CHECK(vars->list);
		vars->type_list = malloc(vars->memsize * sizeof(*(vars->list)));
		MALLOC_NULL_CHECK(vars->type_list);
	}

	if (node->type == IL_OP_NODE) {
		if (node->val.op == IL_ABS_OP || node->val.op == IL_UNI_OP || node->val.op == IL_IN_OP) {
			int64_t var_index = varlist_index(node->children[0].val.str, vars);
			if (var_index  == -1) {
				append_to_varlist(node->children[0].val.str, op_to_var_type(node->val.op), vars);																																		
			} else {
				vars->type_list[var_index] = op_to_var_type(node->val.op);
			}
		} 

		if (node->val.op == IL_BIF_OP) {
			int64_t base_index = varlist_index(node->children[0].val.str, vars);
			if (base_index == -1) {
				fprintf(stderr, "Syntax error on line %ld: unrecognized token \'%s\'.\n", node->lineno, node->children[0].val.str);
				is_valid = false;
			}
			if (vars->type_list[base_index] == UNIVERSE) {
				fprintf(stderr, "Error: attempt to bifurcate a universe object on line %ld.\n", node->lineno);
				is_valid = false;
			}
			for (int32_t i = 1; i < 3; i++) {
				int64_t var_index = varlist_index(node->children[i].val.str, vars);
				if (var_index == -1) {
					append_to_varlist(node->children[i].val.str, vars->type_list[base_index], vars);
				} else {
					vars->type_list[var_index] = vars->type_list[base_index];
				}
			}
		}

		/* THIS.DIE() has to skip var verification */
		if (node->val.op == IL_DIE_OP && strcmp(node->children[0].val.str, "THIS") == 0) {
			return true; 
		}
	}

	if (node->type == IL_VAR_NODE) {
		if (varlist_index(node->val.str, vars) == -1) {
			fprintf(stderr, "Syntax error on line %ld: unrecognized token \'%s\'.\n", node->lineno, node->val.str);
			return false;
		}
		return true;
	}

	for (int64_t i = 0; i < node->num_children; i++) {
		 if (!validate_il_node(node->children + i, vars)) {
			 is_valid = false;
		 }
	}

	/* Universe check has to be after var verification so that the var in question is definitely in vars*/
	if (node->type == IL_OP_NODE && node->val.op == IL_DIE_OP && is_valid) {
		if (vars->type_list[varlist_index(node->children[0].val.str, vars)] == UNIVERSE) {
			fprintf(stderr, "Error: attempt to kill a universe object on line %ld.\n", node->lineno);
			is_valid = false;
		}
	}
	
	if (should_free) {
		free_varlist(vars);
		free(vars);
	}

	return is_valid;
}

int64_t varlist_index(char* str, struct varlist* vars) {
	int64_t index = -1;
	for (int64_t i = 0; i < vars->length; i++) {
		if (strcmp(str, vars->list[i]) == 0) {
			index = i;
			break;
		}
	}
	return index;
}

void append_to_varlist(char* str, enum tildeath_object_type type, struct varlist* vars) {
	if (vars->length >= vars->memsize) {
		vars->memsize *= 2;
		vars->list = realloc(vars->list, vars->memsize * sizeof(*(vars->list)));
		MALLOC_NULL_CHECK(vars);
		vars->type_list = realloc(vars->type_list, vars->memsize * sizeof(*(vars->type_list)));
	}
	vars->list[vars->length] = util_strdup(str);
	vars->type_list[vars->length] = type;
	vars->length++;
	return;
}

enum tildeath_object_type op_to_var_type(enum il_operation op) {
	switch (op) {
		case IL_ABS_OP:
			return ABSTRACT;
		case IL_UNI_OP:
			return UNIVERSE;
		case IL_IN_OP:
			return INPUT;
		default:
			return ABSTRACT;
	}
}

void free_varlist(struct varlist* vars) {
	for (int64_t i = 0; i < vars->length; i++) {
		free(vars->list[i]);
	}
}



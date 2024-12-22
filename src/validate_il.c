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
	int64_t cap;
	int64_t len;
	char** list;
	enum tildeath_object_type* type_list;
};

bool validate_il_node(struct il_node* node, struct varlist* vars);
int64_t varlist_index (char* str, struct varlist* vars);
enum tildeath_object_type op_to_var_type(enum il_operation op);
void append_to_varlist(char* str, enum tildeath_object_type op, struct varlist* vars);
void dup_varlist(struct varlist* out, const struct varlist* in);


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
		vars->cap = 10;
		vars->len = 0;
		vars->list = malloc(vars->cap * sizeof(*(vars->list)));
		MALLOC_NULL_CHECK(vars->list);
		vars->type_list = malloc(vars->cap * sizeof(*(vars->list)));
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

	struct ptrarray delayed_children = {.len=0, .cap=0, .array=NULL}; 
	for (int64_t i = 0; i < node->num_children; i++) {
		/* We have to descend into IF blocks at the end in order to do a breadth-first search that correctly handles scope */
		if (node->children[i].type == IL_OP_NODE && node->children[i].val.op == IL_IF_OP) {
			is_valid = is_valid && validate_il_node(node->children[i].children, vars); // validate IF var

			bool has_execute = true;
			struct il_node* last_op = node->children[i].children[1].children + node->children[i].children[1].num_children - 1;
			if (last_op->type == IL_OP_NODE && last_op->val.op == IL_IF_OP) {
				has_execute = last_op->children[1].val.op == IL_BLOCK_OP;
			}
			if (has_execute) {
				is_valid = is_valid && validate_il_node(last_op, vars); // validate EXECUTE
			}

			append_to_ptrarray(&delayed_children, node->children + i);
			continue;
		}

		is_valid = is_valid && validate_il_node(node->children + i, vars);
	}

	/* Universe check has to be after var verification so that the var in question is definitely in vars*/
	if (node->type == IL_OP_NODE && node->val.op == IL_DIE_OP && is_valid) {
		if (vars->type_list[varlist_index(node->children[0].val.str, vars)] == UNIVERSE) {
			fprintf(stderr, "Error: attempt to kill a universe object on line %ld.\n", node->lineno);
			is_valid = false;
		}
	}

	/* Now we can descend further down the tree. */
	for (int64_t i = 0; i < delayed_children.len; i++) {
		struct varlist cur_vars = {.len=0, .cap=0, .list=NULL, .type_list=NULL};
		dup_varlist(&cur_vars, vars);
		is_valid = is_valid && validate_il_node((struct il_node*) delayed_children.array[i], &cur_vars);
		free_varlist(&cur_vars);
	}
	
	free(delayed_children.array);
	if (should_free) {
		free_varlist(vars);
		free(vars);
	}

	return is_valid;
}

int64_t varlist_index(char* str, struct varlist* vars) {
	int64_t index = -1;
	for (int64_t i = 0; i < vars->len; i++) {
		if (strcmp(str, vars->list[i]) == 0) {
			index = i;
			break;
		}
	}
	return index;
}

void append_to_varlist(char* str, enum tildeath_object_type type, struct varlist* vars) {
	if (vars->len >= vars->cap) {
		vars->cap *= 2;
		vars->list = realloc(vars->list, vars->cap * sizeof(*(vars->list)));
		MALLOC_NULL_CHECK(vars);
		vars->type_list = realloc(vars->type_list, vars->cap * sizeof(*(vars->type_list)));
	}
	vars->list[vars->len] = util_strdup(str);
	vars->type_list[vars->len] = type;
	vars->len++;
	return;
}

void dup_varlist(struct varlist* out, const struct varlist* in) {
	out->len = in->len;
	out->cap = in->cap;
	out->list = malloc(out->cap * sizeof(*(out->list)));
	MALLOC_NULL_CHECK(out->list);
	out->type_list = malloc(out->cap * sizeof(*(out->type_list)));
	MALLOC_NULL_CHECK(out->type_list);
	for (int64_t i = 0; i < out->len; i++) {
		out->list[i] = util_strdup(in->list[i]);
		out->type_list[i] = in->type_list[i];
	}
}

void free_varlist(struct varlist* vars) {
	for (int64_t i = 0; i < vars->len; i++) {
		free(vars->list[i]);
	}
	free(vars->list);
	free(vars->type_list);
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

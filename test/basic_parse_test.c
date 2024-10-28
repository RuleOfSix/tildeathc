#include <parse.h>
#include <project_info.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

#ifndef TEST_INPUT_DIR 
#define TEST_INPUT_DIR "."
#endif

bool compare(ast* expected, ast* actual);

int32_t main() {
	token input_tokens[] = {
		{"import", 1},
		{"library", 1},
		{"NUMBERS", 1},
		{";", 1},
		{"import", 2},
		{"abstract", 2},
		{"LAMB", 2},
		{";", 2},
		{"~ATH", 4},
		{"(", 4},
		{"LAMB", 4},
		{")", 4},
		{"{", 4},
		{"LAMB", 5},
		{".DIE", 5},
		{"(", 5},
		{")", 5},
		{";", 5},
		{"}", 6},
		{"EXECUTE", 6},
		{"(", 6},
		{"PRINT", 6},
		{"\"HELLO WORLD!\"", 6},
		{";", 6},
		{")", 6},
		{";", 6},
		{"THIS", 7},
		{".DIE", 7},
		{"(", 7},
		{")", 7},
		{";", 7}
	};
	token_list input = {input_tokens, 31};

	ast grave_1_c[] = {{.type=STRING_NODE, .val.str="library", .num_children=0, .children=NULL},
					   {.type=STRING_NODE, .val.str="NUMBERS", .num_children=0, .children=NULL}};
	ast grave_1 = {.type=OPERATION_NODE, .val.op=IMPORT_OP, .num_children=2, .children=grave_1_c};
	ast grave_2_c[] = {{.type=STRING_NODE, .val.str="abstract", .num_children=0, .children=NULL},
					   {.type=STRING_NODE, .val.str="LAMB", .num_children=0, .children=NULL}};
	ast grave_3_c_2_c[] = {{.type=STRING_NODE, .val.str="LAMB", .num_children=0, .children=NULL}};
	ast grave_3_c_3_c[] = {{.type=STRING_NODE, .val.str="HELLO WORLD!", .num_children=0, .children=NULL}};
	ast grave_3_c[] = {{.type=STRING_NODE, .val.str="LAMB", .num_children=0, .children=NULL},
					 {.type=OPERATION_NODE, .val.op=DIE_OP, .num_children=1, .children=grave_3_c_2_c},
					 {.type=OPERATION_NODE, .val.op=PRINT_OP, .num_children=1, .children=grave_3_c_3_c}};
	ast grave_3 = {.type=OPERATION_NODE, .val.op=LOOP_OP, .num_children=3, .children=grave_3_c};
	ast grave_2 = {.type=OPERATION_NODE, .val.op=IMPORT_OP, .num_children=2, .children=grave_2_c};
	ast grave_4_c[] = {{.type=STRING_NODE, .val.str="THIS", .num_children=0, .children=NULL}};
	ast grave_4 = {.type=OPERATION_NODE, .val.op=DIE_OP, .num_children=1, .children=grave_4_c};
	ast root_c[] = {grave_1, grave_2, grave_3, grave_4};
	ast expected_output = {.type=ROOT_NODE, .val.str=NULL, .num_children=4, .children=root_c};

	ast* output = parse(&input);
	if (output == NULL) {
		fprintf(stderr, "Test failed: parse output null\n");
		exit(EXIT_FAILURE);
	}
	if (!compare(&expected_output, output)) {
		fprintf(stderr, "Test failed: parse output does not match expected AST\n");
		exit(EXIT_FAILURE);
	}
	printf("Basic parse module test passed\n");
	return 0;
}

bool compare(ast* expected, ast* actual) {
	if (expected->type != actual->type) {
		return false;
	}
	if (expected->type == STRING_NODE) {
		if (strcmp(expected->val.str, actual->val.str) != 0) {
			return false;
		}
	}
	if (expected->type == OPERATION_NODE) {
		if (expected->val.op != actual->val.op) {
			 return false;
		}
	}
	if (expected->num_children != actual->num_children) {
		return false;
	}
	for (int64_t i = 0; i < expected->num_children; i++) {
		if (!compare(&(expected->children[i]), &(actual->children[i]))) {
			return false;	
		}
	}
	return true;
}

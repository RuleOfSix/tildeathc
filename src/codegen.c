#include <codegen.h>
#include <registers.h>
#include <il.h>
#include <nullcheck.h>
#include <util.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <string.h>

int32_t process_node(const struct il_node* node, FILE* output, const struct strarray* strings, struct strarray* var_table);
int32_t process_if(const struct il_node* node, FILE* output, const struct strarray* strings, struct strarray* var_table);
int32_t process_block(const struct il_node* node, FILE* output, const struct strarray* strings, struct strarray* var_table);
int32_t process_jmp(const struct il_node* node, FILE* output);
int32_t process_bif(const struct il_node* node, FILE* output, struct strarray* var_table);
int32_t process_die(const struct il_node* node, FILE* output, struct strarray* var_table);
int32_t process_out(const struct il_node* node, FILE* output, const struct strarray* strings);
int32_t process_abs(const struct il_node* node, FILE* output, struct strarray* var_table);
int32_t process_uni(const struct il_node* node, FILE* output, struct strarray* var_table);
int32_t process_in(const struct il_node* node, FILE* output, struct strarray* var_table);
int32_t process_lib(const struct il_node* node, FILE* output, struct strarray* var_table);
int64_t declare_var(char* varname, FILE* output, struct strarray* var_table, int64_t guard_offset);
int32_t get_var_table(const struct il_node* il, struct strarray* output);
int64_t get_offset(char* varname, struct strarray* var_table);
int32_t get_strings(const struct il_node* il, struct strarray* output);
char* get_strlabel(char* str, const struct strarray* strings);
char* format_label(int64_t num, char* prefix);

int32_t generate_assembly(const struct il_node* il, char* filename, FILE* output) {
	if (output == NULL) {
		fprintf(stderr, "Internal Error: cannot write to NULL file pointer.\n");
		exit(EXIT_FAILURE);
	}
	if (il == NULL) {
		fprintf(stderr, "Internal Error: cannot generate assembly from NULL il tree.\n");
		exit(EXIT_FAILURE);
	}
	if (il->type != IL_ROOT_NODE) {
		fprintf(stderr, "Internal Error: cannot generate assembly from non-root il node.\n");
		exit(EXIT_FAILURE);
	}
	if (filename == NULL) {
		fprintf(stderr, "Internal Error: NULL filename passed to generate_assembly.\n");
		exit(EXIT_FAILURE);
	}

	struct strarray strings = {.array=NULL, .len=0, .cap=0};
	if (get_strings(il, &strings) == 1) {
		fprintf(stderr, "Internal Error: unknown problem while fetching strings from il tree.\n");
		exit(EXIT_FAILURE);
	}
	
	struct strarray var_table = {.array=NULL, .len=0, .cap=0};
	if (get_var_table(il, &var_table) == 1) {
		fprintf(stderr, "Internal Error: unknown problem while building variable table from il tree.\n");
		exit(EXIT_FAILURE);
	}
		
	fprintf(output, "\t.file\t\"%s\"\n", filename);	
	if (strings.len > 0) {
		fprintf(output, "\t.text\n");
		fprintf(output, "\t.section\t.rodata\n");
		for (int64_t i = 0; i < strings.len; i++) {
			fprintf(output, ".LS%ld:\n", i);
			fprintf(output, "\t.string \"%s\"\n", strings.array[i]);
		}
	}
	fprintf(output, "\t.text\n");
	fprintf(output, "\t.globl\tmain\n");
	fprintf(output, "\t.type\tmain, @function\n");
	fprintf(output, "main:\n");
	fprintf(output, "\tpushq\t%%rbp\n");
	fprintf(output, "\tmovq\t%%rsp, %%rbp\n");
	fprintf(output, "\tsubq\t$8, %%rsp\n");
	fprintf(output, "\tpushq\t%%r12\n");
	fprintf(output, "\tmovq\t$%ld,%%rdi\n", var_table.len);
	fprintf(output, "\tcall\tallocate_progvars@PLT\n");
	fprintf(output, "\tmovq\t%%rax, %%r12\n");

	var_table.len = 0;
	var_table.cap = 2;
	free(var_table.array); 
	var_table.array = malloc(sizeof(*(var_table.array)) * var_table.cap);
	MALLOC_NULL_CHECK(var_table.array);

	for (int64_t i = 0; i < il->num_children; i++) {
		if (process_node(il->children + i, output, &strings, &var_table) == 1) {
			return 1;
		}
	}

	return 0;
}

int32_t process_node(const struct il_node* node, FILE* output, const struct strarray* strings, struct strarray* var_table) {
	if (node->type != IL_OP_NODE) {
		return 1;
	}
	int32_t status = 0;
	char* label = format_label(node->id, "L");
	fprintf(output, "%s:\n", label);
	free(label);
	switch (node->val.op) {
		case IL_IF_OP:
			status = process_if(node, output, strings, var_table);
			break;
		case IL_BLOCK_OP:
			status = process_block(node, output, strings, var_table);
			break;
		case IL_JMP_OP:
			status = process_jmp(node, output);
			break;
		case IL_BIF_OP:
			status = process_bif(node, output, var_table);
			break;
		case IL_DIE_OP:
			status = process_die(node, output, var_table);
			break;
		case IL_OUT_OP:
			status = process_out(node, output, strings);
			break;
		case IL_ABS_OP:
			status = process_abs(node, output, var_table);
			break;
		case IL_LIB_OP:
			status = process_lib(node, output, var_table);
			break;
		case IL_UNI_OP:
			status = process_uni(node, output, var_table);
			break;
		case IL_IN_OP:
			status = process_in(node, output, var_table);
			break;
	}
	return status;
}

int32_t process_if(const struct il_node* node, FILE* output, const struct strarray* strings, struct strarray* var_table) {
	int64_t var_offset = get_offset(node->children[0].val.str, var_table);
	enum cpu_register reg1 = allocate_register();
	enum cpu_register reg2 = allocate_register();
	char* reg1_str = register_name(reg1, B64);
	char* reg2_str = register_name(reg2, B32);
	char* end_label = format_label(node->id, "LE");

	fprintf(output, "\tmovq\t%ld(%%r12), %s\n", var_offset, reg1_str);
	fprintf(output, "\tmovl\t4(%s), %s\n", reg1_str, reg2_str);
	fprintf(output, "\ttest\t%s, %s\n", reg2_str, reg2_str);
	fprintf(output, "\tjnz\t%s\n", end_label);
	free_register(reg1);
	free_register(reg2);
	free(reg1_str);
	free(reg2_str);

	process_node(node->children + 1, output, strings, var_table);

	fprintf(output, "%s:\n", end_label);
	free(end_label);
	return 0;
}


int32_t process_block(const struct il_node* node, FILE* output, const struct strarray* strings, struct strarray* var_table) {
	int32_t status = 0;
	struct strarray* scope_var_table = dup_strarray(var_table);
	for (int64_t i = 0; i < node->num_children; i++) { 
		bool is_execute = i == node->num_children - 1 &&
						  (node->children[i].val.op != IL_IF_OP ||
						  node->children[i].children[1].val.op == IL_BLOCK_OP);
		struct strarray* cur_var_table = is_execute ? var_table : scope_var_table;
		status = status || process_node(node->children + i, output, strings, cur_var_table);
	}
	for (int64_t i = var_table->len; i < scope_var_table->len; i++) {
		fprintf(output, "\tmovq\t%ld(%%r12), %%rdi\n", i * 8);
		fprintf(output, "\tcall\tfree_object@PLT\n");
	}
	return status;
}


int32_t process_jmp(const struct il_node* node, FILE* output) {
	char* target_label = format_label(node->children[0].val.tar, "L");
	fprintf(output, "\tjmp\t%s\n", target_label);
	free(target_label);
	return 0;
}

int32_t process_bif(const struct il_node* node, FILE* output, struct strarray* var_table) {
	int64_t source_offset = get_offset(node->children[0].val.str, var_table);
	int64_t left_offset = declare_var(node->children[1].val.str, output, var_table, source_offset);
	int64_t right_offset = declare_var(node->children[2].val.str, output, var_table, source_offset);

	bool free_source = left_offset == source_offset || right_offset == source_offset;

	fprintf(output, "\tmovq\t%ld(%%r12), %%rdi\n", source_offset);
	if (free_source) {
		fprintf(output, "\tpushq\t%%rdi\n");
	}
	fprintf(output, "\tcall\tbifurcate@PLT\n");

	enum cpu_register reg = allocate_register();
	char* reg_str = register_name(reg, B64);

	fprintf(output, "\tmovq\t(%%rax), %s\n", reg_str);
	fprintf(output, "\tmovq\t%s, %ld(%%r12)\n", reg_str, left_offset);
	fprintf(output, "\tmovq\t8(%%rax), %s\n", reg_str);
	fprintf(output, "\tmovq\t%s, %ld(%%r12)\n", reg_str, right_offset);

	if (free_source) {
		fprintf(output, "\tpopq\t%%rdi\n");
		fprintf(output, "\tcall\tfree_object@PLT\n");
	}

	free(reg_str);
	free_register(reg);
	
	return 0;
}

int32_t process_out(const struct il_node* node, FILE* output, const struct strarray* strings) {
	char* str_label = get_strlabel(node->children[0].val.str, strings);
	if (str_label == NULL) {
		fprintf(stderr, "Compiler error: unable to locate string %s", node->children[0].val.str);
	}
	fprintf(output, "\tleaq\t%s(%%rip), %%rdi\n", str_label);
	fprintf(output, "\tcall\tprint@PLT\n");
	return 0;
}

int32_t process_die(const struct il_node* node, FILE* output, struct strarray* var_table) {
	if (strcmp(node->children[0].val.str, "THIS") == 0) {
		fprintf(output, "\tpopq\t%%r12\n");
		fprintf(output, "\tmovl\t$0, %%eax\n");
		fprintf(output, "\tleave\n");
		fprintf(output, "\tret\n");
	} else {
		for (int64_t i = 0; i < node->num_children; i++) {
			int64_t var_offset = get_offset(node->children[i].val.str, var_table);
			fprintf(output, "\tmovq\t%ld(%%r12), %%rdi\n", var_offset);
			fprintf(output, "\tcall\tkill@PLT\n");
		}
	}
	return 0;
}

int32_t process_abs(const struct il_node* node, FILE* output, struct strarray* var_table) {
	int64_t var_offset = declare_var(node->children[0].val.str, output, var_table, 0);
	fprintf(output, "\tmovl\t$0, %%edi\n");
	fprintf(output, "\tcall\tcreate_object@PLT\n");
	fprintf(output, "\tmovq\t%%rax, %ld(%%r12)\n", var_offset);
	return 0;
}

int32_t process_uni(const struct il_node* node, FILE* output, struct strarray* var_table) {
	int64_t var_offset = declare_var(node->children[0].val.str, output, var_table, 0);
	fprintf(output, "\tmovl\t$1, %%edi\n");
	fprintf(output, "\tcall\tcreate_object@PLT\n");
	fprintf(output, "\tmovq\t%%rax, %ld(%%r12)\n", var_offset);
	return 0;

}

int32_t process_in(const struct il_node* node, FILE* output, struct strarray* var_table) {
	int64_t var_offset = declare_var(node->children[0].val.str, output, var_table, 0);
	fprintf(output, "\tmovl\t$2, %%edi\n");
	fprintf(output, "\tcall\tcreate_object@PLT\n");
	fprintf(output, "\tmovq\t%%rax, %ld(%%r12)\n", var_offset);
	return 0;
}


int32_t process_lib(const struct il_node* node, FILE* output, struct strarray* var_table) {
	int32_t status = 0;
	for (int64_t i = 0; i < node->num_children; i++) {
		status = status || process_node(node->children + i, output, NULL, var_table);
	}
	return status;
}

int64_t declare_var(char* varname, FILE* output, struct strarray* var_table, int64_t guard_offset) {
	int64_t offset = get_offset(varname, var_table);
	if (offset != -1) {
		if (offset == guard_offset) {
			return offset;
		}
		fprintf(output, "\tmovq\t%ld(%%r12), %%rdi\n", offset);
		fprintf(output, "\tcall\tfree_object@PLT\n");
	} else {
		append_to_strarray(var_table, varname, false);
		offset = 8 * (var_table->len - 1);
	}
	return offset;
}

int32_t get_var_table(const struct il_node* il, struct strarray* output) {
	if (output == NULL) {
		return 1;
	}

	if (il->type == IL_DEC_NODE && get_offset(il->val.str, output) == -1) {
		append_to_strarray(output, il->val.str, false);
		return 0;
	}

	for (int64_t i = 0; i < il->num_children; i++) {
		if (get_var_table(il->children + i, output) == 1) {
			return 1;
		}
	}

	return 0;
}

int64_t get_offset(char* varname, struct strarray* var_table) {
	int64_t offset = -1;
	for (int64_t i = 0; i < var_table->len; i++) {
		if (strcmp(var_table->array[i], varname) == 0) {
			offset = 8 * i;
			break;
		}
	}
	return offset;
}

int32_t get_strings(const struct il_node* il, struct strarray* output) {
	if (output == NULL) {
		return 1;
	}

	if (il->type == IL_STR_NODE && get_offset(il->val.str, output) == -1) {
		append_to_strarray(output, il->val.str, false);
		return 0;
	} 

	for (int64_t i = 0; i < il->num_children; i++) {
		if (get_strings(il->children + i, output) == 1) {
			return 1;
		}
	}

	return 0;
}

char* get_strlabel(char* str, const struct strarray* strings) {
	for (int64_t i = 0; i < strings->len; i++) {
		if (strcmp(strings->array[i], str) == 0) {
			return format_label(i, "LS");
		}
	}
	return NULL;
}

char* format_label(int64_t num, char* prefix) {
	char* output = malloc(10 * sizeof(*output));
	MALLOC_NULL_CHECK(output);
	snprintf(output, 10, ".%s%ld", prefix, num);
	return output;
}

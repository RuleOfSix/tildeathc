#include <project_info.h>
#include <tildeathc_test.h>
#include <nullcheck.h>
#include <tokenize.h>
#include <parse.h>
#include <il.h>
#include <codegen.h>
#include <errno.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/wait.h>



void tokenize_test(FILE* input, const struct token_list* expected, bool should_fail) {
	ASSERT_NOT_NULL(input);	
	if (!should_fail) {
		ASSERT_NOT_NULL(expected);	
	}
	struct token_list* output = tokenize(input);
	if (should_fail) {
		fprintf(stderr, "Tokenize test failed: tokenize did not fail with incorrect input.\n");
		exit(EXIT_SUCCESS);
	}
	if (output == NULL) {
		fprintf(stderr, "Tokenize test failed: tokenize output null.\n");
		exit(EXIT_FAILURE);
	}
	if (!compare_token_list(expected, output)) {
		fprintf(stderr, "Tokenize test failed: output token list does not match what was expected.\n");
		exit(EXIT_FAILURE);
	}
	free_token_list(output);
	free(output);
}

void parse_test(const struct token_list* input, const struct ast* expected, bool should_fail) {
	ASSERT_NOT_NULL(input);
	if (!should_fail) {
		ASSERT_NOT_NULL(expected);
	}
	struct ast* output = parse(input, false);
	if (should_fail) {
		fprintf(stderr, "Parse test failed: parse did not fail with incorrect input.\n");
		exit(EXIT_SUCCESS);
	}
	if (output == NULL) {
		fprintf(stderr, "Parse test failed: parse output null.\n");
		exit(EXIT_FAILURE);	
	}	
	if (!compare_ast(expected, output)) {
		fprintf(stderr, "Parse test failed: output AST does not match what was expected.\n");
		exit(EXIT_FAILURE);
	}
	free_ast(output);
	free(output);
}

void il_test(const struct ast* input, const struct il_node* expected, bool should_fail) {
	ASSERT_NOT_NULL(input);
	if (!should_fail) {
		ASSERT_NOT_NULL(expected);
	}
	struct il_node* output = generate_il(input, false, NULL);
	if (should_fail) {
		fprintf(stderr, "IL test failed: generate_il did not fail with incorrect input.\n");
		exit(EXIT_SUCCESS);
	}
	if (output == NULL) {
		fprintf(stderr, "IL test failed: generate_il output null.\n");
		exit(EXIT_FAILURE);
	}
	if (!compare_il_tree(expected, output)) {
		fprintf(stderr, "IL test failed: output tree does not match what was expected.\n");
		exit(EXIT_FAILURE);
	}
	free_il_tree(output);
	free(output);
}

void codegen_test(const struct il_node* input, char* expected, bool should_fail) {
	ASSERT_NOT_NULL(input);
	if (!should_fail) {
		ASSERT_NOT_NULL(expected);
	}

	errno = 0;
	char asm_filename[] = "/tmp/tildeathc_XXXXXX.s";
	int asm_file_fd = mkstemps(asm_filename, 2);
	if (asm_file_fd == -1) {
		fprintf(stderr, "Temporary file %s creation failed: %s\n", asm_filename, strerror(errno));
		exit(!should_fail);
	}

	errno = 0;
	FILE* asm_file = fdopen(asm_file_fd, "w");
	if (asm_file == NULL) {
		fprintf(stderr, "Opening %s file stream failed: %s\n", asm_filename, strerror(errno));
		unlink(asm_filename);
		exit(!should_fail);
	}

	if (generate_assembly(input, "Test.~ATH", asm_file) == 1) {
		fprintf(stderr, "Assembly generation failed.\n");
		exit(!should_fail);
	}

	errno = 0;
	if (fclose(asm_file) == EOF) {
		fprintf(stderr, "Error: Failed to close file %s: %s\n", asm_filename, strerror(errno));
		exit(!should_fail);
	}

	char bin_filename[] = "/tmp/tildeathc_XXXXXX";
	errno = 0;
	int unused_fd = mkstemp(bin_filename);
	if (unused_fd == -1) {
		fprintf(stderr, "Temporary file %s creation failed: %s\n", bin_filename, strerror(errno));
		exit(!should_fail);
	}

	errno = 0;
	if (close(unused_fd) == -1) {
		fprintf(stderr, "Warning: Failed to close unused file descripter for %s: %s\n", bin_filename, strerror(errno));
	}

	errno = 0;
	int32_t pid = fork();
	if (pid < 0) {
		perror("Error attempting to fork process");
		exit(!should_fail);
	} else if (pid == 0) {
		errno = 0;
		execlp("gcc", "gcc", asm_filename, "-L" RUNTIME_LIB_DIR, "-L" SRC_LIB_DIR, "-lobjects", "-lutil", "-o", bin_filename, (char*) NULL);
		if (errno == 0) { 
			fprintf(stderr, "Execlp call for gcc failed.\n");
		} else {
			perror("Execlp failure for gcc");
		}
		unlink(asm_filename);
		unlink(bin_filename);
		exit(!should_fail);
	} 
	int status = 0;
	errno = 0;
	if (waitpid(pid, &status, 0) != pid) {
		if (errno == 0) {
			fprintf(stderr, "Unknown waitpid error.\n");
		} else {
			perror("Waitpid error");
		}
		unlink(asm_filename);
		unlink(bin_filename);
		exit(!should_fail);
	}
	if (status != 0) {
		fprintf(stderr, "Error: gcc exited with status code %d\n", status);
		fprintf(stderr, "Assembly file stored at %s for inspection.\n", asm_filename);
		unlink(bin_filename);
		exit(!should_fail);
	}
	
	errno = 0;
	FILE* program_output = popen(bin_filename, "r");
	if (program_output == NULL) {
		if (errno == 0) {
			fprintf(stderr, "Popen error: Memory allocation failure attempting to execute generated binary.\n");
		} else {
			perror("Popen error executing generated binary");
		}
		unlink(asm_filename);
		unlink(bin_filename);
		exit(!should_fail);
	}
	char* program_output_line = NULL;
	size_t program_output_line_size = 0;
	int32_t i = 1;
	for (char* expected_line = strtok(expected, "\n"); expected_line != NULL; expected_line = strtok(NULL, "\n")) {
		getline(&program_output_line, &program_output_line_size, program_output);
		program_output_line[strlen(program_output_line) - 1] = '\0';
		fprintf(stderr, "\tExpected: %s\n", expected_line);
		fprintf(stderr, "\tActual: %s\n", program_output_line);
		if (strcmp(program_output_line, expected_line) != 0) {
			fprintf(stderr, "Error: program output line %d does not match what was expected.\n", i);
			fprintf(stderr, "Assembly file stored at %s for inspection.\n", asm_filename);
			unlink(bin_filename);
			exit(!should_fail);
		}
		i++;
	}

	printf("Code generation test passed.\n");

	unlink(asm_filename);
	unlink(bin_filename);
	pclose(program_output);
	exit(should_fail);
}

bool compare_token_list(const struct token_list* expected, const struct token_list* actual) {
	if (actual->length != expected->length) {
		fprintf(stderr, "Test failed: expected length property of %ld, output was %ld.\n", expected->length, actual->length);
		return false;
	}

	for (int64_t i = 0; i < actual->length; i++) {
		if (strcmp(actual->tokens[i].str, expected->tokens[i].str) != 0) {
			fprintf(stderr, "Test failed: expected token %ld's str to be %s, output was %s.\n", i, expected->tokens[i].str, actual->tokens[i].str);
			return false;
		}
		if (actual->tokens[i].lineno != expected->tokens[i].lineno) {
			fprintf(stderr, "Test failed: expected token %ld's lineno to be %ld, output was %ld.\n", i, expected->tokens[i].lineno, actual->tokens[i].lineno);
			return false;
		}
	}
	return true;
}

bool compare_ast(const struct ast* expected, const struct ast* actual) {
	if (expected->type != actual->type) {
		fprintf(stderr, "Expected node type of %s, got %s on line %ld.\n", LOOKUP_NODE_TYPE(expected->type), LOOKUP_NODE_TYPE(actual->type), actual->lineno);
		return false;
	}
	if (expected->lineno != actual->lineno) {
		fprintf(stderr, "Expected line number of %ld, got %ld for node of type %s.\n", expected->lineno, actual->lineno, LOOKUP_NODE_TYPE(actual->type));
		return false;
	}
	if (expected->type == STRING_NODE) {
		if (strcmp(expected->val.str, actual->val.str) != 0) {
			fprintf(stderr, "Expected string value of \"%s\", got \"%s\" on line %ld.\n", expected->val.str, actual->val.str, actual->lineno);
			return false;
		}
	}
	if (expected->type == OPERATION_NODE) {
		if (expected->val.op != actual->val.op) {
			fprintf(stderr, "Expected operation value of %s, got %s on line %ld.\n", LOOKUP_OPERATION(expected->val.op), LOOKUP_OPERATION(actual->val.op), actual->lineno);
			return false;
		}
	}
	if (expected->num_children != actual->num_children) {
		fprintf(stderr, "Expected %ld children, got %ld on line %ld.\n", expected->num_children, actual->num_children, actual->lineno);
		return false;
	}
	for (int64_t i = 0; i < expected->num_children; i++) {
		if (!compare_ast(expected->children + i, actual->children + i)) {
			return false;	
		}
	}
	return true;
}

bool compare_il_tree(const struct il_node* expected, const struct il_node* actual) {
	if (expected->id != actual->id) {
		fprintf(stderr, "Expected id of %ld, got %ld on line %ld.\n", expected->id, actual->id, actual->lineno);
		return false;
	}
	if (expected->lineno != actual->lineno) {
		fprintf(stderr, "Expected line number of %ld, got %ld for node with id %ld.\n", expected->lineno, actual->lineno, actual->id);
		return false;
	}
	if (expected->type != actual->type) {
		fprintf(stderr, "Expected type of %s, got %s on line %ld.\n", LOOKUP_IL_TYPE(expected->type), LOOKUP_IL_TYPE(actual->type), actual->lineno);
		return false;
	}
	switch (actual->type) {
		case IL_TAR_NODE:
			if (expected->val.tar != actual->val.tar) {
				fprintf(stderr, "Expected target of %ld, got %ld on line %ld.\n", expected->val.tar, actual->val.tar, actual->lineno);
				return false;
			}
			break;
		case IL_DEC_NODE: //FALLTHROUGH
		case IL_STR_NODE: //FALLTHROUGH
		case IL_VAR_NODE:
			if (strcmp(expected->val.str, actual->val.str) != 0) {
				fprintf(stderr, "Expected string value of \"%s\", got \"%s\" on line %ld.\n", expected->val.str, actual->val.str, actual->lineno);
				return false;
			}
			break;
		case IL_OP_NODE:
			if (expected->val.op != actual->val.op) {
				fprintf(stderr, "Expected operation value of %s, got %s on line %ld.\n", LOOKUP_IL_OPERATION(expected->val.op), LOOKUP_IL_OPERATION(actual->val.op), actual->lineno);
			}
			break;
		case IL_ROOT_NODE:
			break;

	}
	if (expected->num_children != actual->num_children) {
		fprintf(stderr, "Expected %ld children, got %ld on line %ld.\n", expected->num_children, actual->num_children, actual->lineno);
		exit(EXIT_FAILURE);
	}
	for (int64_t i = 0; i < actual->num_children; i++) {
		if (!compare_il_tree(expected->children + i, actual->children + i)) {
			return false;
		}
	}
	return true;
}

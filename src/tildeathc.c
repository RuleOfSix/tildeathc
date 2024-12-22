#include<project_info.h>
#include<tokenize.h>
#include<parse.h>
#include<il.h>
#include<validate_il.h>
#include<codegen.h>
#include<nullcheck.h>
#include<util.h>
#include<stdio.h>
#include<stdint.h>
#include<stdlib.h>
#include<unistd.h>
#include<getopt.h>
#include<errno.h>

int32_t main(int32_t argc, char* argv[]) {
	if (argc == 1) {
		printf("Usage: tildeathc [-S] [-e] [-h] [-v] [-o output_file] input_file\n");
		exit(EXIT_FAILURE);
	}
	char* output_filename = NULL;
	char default_output_filename[] = "a.out";
	char default_asm_output_filename[] = "a.s";
	struct strarray include_dirs = {.len=0, .cap=0, .array=NULL};
	append_to_strarray(&include_dirs, "/usr/include/tildeathc", true);
	bool output_asm = false;
	bool echo_output = false;
	opterr = 0;
	for (char opt = getopt(argc, argv, "SI:eo:hv"); opt != -1; opt = getopt(argc, argv, "SI:eo:hv")) {
		switch (opt) {
			case 'S':
				output_asm = true;
				break;
			case 'e':
				echo_output = true;
				break;
			case 'o':
				output_filename = optarg;
				break;
			case 'I':
				append_to_strarray(&include_dirs, optarg, true);
				break;
			case 'h':
				printf("\ntildeathc is a compiler for ruleofsix's version of the ~ATH programming language. It will not compile any other form of ~ATH,\n");
				printf("and it currently can only compile to amd64 Linux systems. If you're having problems or have any suggestions, feel free to email\n");
				printf("the creator at github@ruleofsix.ca\n\n");
				printf("Usage:\n\n\ttildeathc [-S] [-e] [-h] [-v] [-o output_file] input_file\n");
				printf("Options:\n\n");
				printf("\t-S\t\t\tOutput assembly code instead of an executable binary\n");
				printf("\t-e\t\t\tEcho output to stdout instead of writing to a file (only usable with -S, ignores -o)\n");
				printf("\t-h\t\t\tDisplay help message and exit\n");
				printf("\t-v\t\t\tDisplay version information and exit\n");
				printf("\t-o output_file\t\tPath to file where output binary should be written (default: a.out)\n");
				printf("\n");
				exit(EXIT_SUCCESS);
				break;
			case 'v':
				printf("tildeathc - ~ATH compiler by ruleofsix, version "  TILDEATHC_VERSION "\n");
				exit(EXIT_SUCCESS);
				break;
			case '?':
				printf("Usage: tildeathc [-S] [-e] [-o output_file] input_file\n");
				exit(EXIT_FAILURE);
				break;
		}
	}
	if (output_filename == NULL) {
		output_filename = output_asm ? default_asm_output_filename : default_output_filename;
	}

	if (echo_output && !output_asm) {
		fprintf(stderr, "Error: -e option cannot be used with machine code output; please use it with -S instead.\n");
		exit(EXIT_FAILURE);
	}
	if (optind < argc - 1) {
		for (int i = optind + 1; i < argc; i++) {
			fprintf(stderr, "Error: unrecognized argument \"%s\"\n", argv[i]);
		}
		exit(EXIT_FAILURE);
	}
	if (optind > argc - 1) {
		fprintf(stderr, "Error: missing input file argument.\n");
		exit(EXIT_FAILURE);
	}

	char* input_filename = argv[optind];
	errno = 0;
	FILE* input_file = fopen(input_filename, "r");
	if (input_file == NULL) {
		perror("Error reading input file");
		exit(EXIT_FAILURE);
	}

	struct token_list* tokens = tokenize(input_file);
	struct ast* syntax_tree = parse(tokens, false);
	free_token_list(tokens);
	free(tokens);
	struct il_node* il_tree = generate_il(syntax_tree, false, &include_dirs);
	free_ast(syntax_tree);
	free(syntax_tree);
	if (!validate_il(il_tree)) {
		exit(EXIT_FAILURE);
	}

	FILE* asm_file = stdout;
	char asm_filename[] = "/tmp/tildeathc_XXXXXX.s";
	if (!echo_output) {
		errno = 0;
		if (!output_asm) {
			int asm_file_d = mkstemps(asm_filename, 2);
			if (asm_file_d == -1) {
				perror("Error creating temporary file");
				exit(EXIT_FAILURE);
			}
			errno = 0;
			asm_file = fdopen(asm_file_d, "w");
		} else {
			asm_file = fopen(output_filename, "w");
		}
	}
	if (asm_file == NULL) {
		perror("Error opening temporary file");
		exit(EXIT_FAILURE);
	}

	generate_assembly(il_tree, input_filename, asm_file);
	
	if (output_asm) {
		if (asm_file != stdout) {
			fclose(asm_file);
		}
		exit(EXIT_SUCCESS);
	}
	fclose(asm_file);
	
	errno = 0;
	FILE* output_file = fopen(output_filename, "w");
	if (output_file == NULL) {
		perror("Error creating output file");
		unlink(asm_filename);
		exit(EXIT_FAILURE);
	}
	
	execlp("gcc", "gcc", asm_filename, "-L" RUNTIME_LIB_DIR, "-L" SRC_LIB_DIR, "-lobjects", "-lutil", "-o", output_filename, (char*) NULL);
	if (errno == 0) { 
		fprintf(stderr, "Internal Error: execlp failed trying to execute gcc\n");
	} else {
		perror("Internal execlp error trying to execute gcc");
	}
	unlink(asm_filename);
	unlink(output_filename);
	exit(EXIT_FAILURE);

}

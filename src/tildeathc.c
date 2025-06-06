#include <project_info.h>
#include <tokenize.h>
#include <parse.h>
#include <il.h>
#include <validate_il.h>
#include <codegen.h>
#include <nullcheck.h>
#include <util.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>
#include <errno.h>
#include <signal.h>
#include <sys/wait.h>

char* asm_filename = NULL;
char* output_filename = NULL;
void unlink_asmfile();
void unlink_outfile();

int32_t main(int32_t argc, char* argv[]) {
	const char* usage_str = "Usage: tildeathc [-S] [-e] [-r] [-h] [-v] [-o output_file] [-I include_directory] input_file";
	if (argc == 1) {
		printf("%s\n", usage_str);
		exit(EXIT_FAILURE);
	}
	const char* tmpdir_env = secure_getenv("TMPDIR");
	const char* tmpdir = tmpdir_env ? tmpdir_env : "/tmp";
	char default_output_filename[] = "a.out";
	char default_asm_output_filename[] = "a.s";
	struct strarray include_dirs = {.len=0, .cap=0, .array=NULL};
	append_to_strarray(&include_dirs, TILDEATH_LIB_INSTALL_DIR, true);
	bool output_asm = false;
	bool echo_output = false;
	bool run = false;
	opterr = 0;
	for (char opt = getopt(argc, argv, "SI:ero:hv"); opt != -1; opt = getopt(argc, argv, "SI:ero:hv")) {
		switch (opt) {
			case 'S':
				output_asm = true;
				break;
			case 'e':
				echo_output = true;
				break;
			case 'r':
				output_filename = util_strcat(util_strdup(tmpdir), "/tildeathc_XXXXXX");

				errno = 0;
				int unused_fd = mkstemp(output_filename);
				if (unused_fd == -1) {
					fprintf(stderr, "Temporary file %s creation failed: %s\n", output_filename, strerror(errno));
					exit(EXIT_FAILURE);
				}

				errno = 0;
				if (close(unused_fd) == -1) {
					fprintf(stderr, "Warning: Failed to close unused file descripter for %s: %s\n", output_filename, strerror(errno));
				}

				run = true;
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
				printf("the creator at github@ruleofsix.ca.\n\n");
				printf("%s\n", usage_str);
				printf("Options:\n\n");
				printf("\t-S\t\t\tOutput assembly code instead of an executable binary\n");
				printf("\t-e\t\t\tEcho asm to stdout instead of writing to a file (implies -S, ignores -o)\n");
				printf("\t-r\t\t\tRun program immediately without producing an output file. Ignores -S, -e, and -o.\n");
				printf("\t-h\t\t\tDisplay help message and exit\n");
				printf("\t-v\t\t\tDisplay version information and exit\n");
				printf("\t-o output_file\t\tPath to file where output binary should be written (default: a.out)\n");
				printf("\t-I include_dir\t\tPath to directory for library includes. Can be used more than once, current directory and /usr/include/tildeathc are searched by default.\n");
				printf("\n");
				exit(EXIT_SUCCESS);
				break;
			case 'v':
				printf("tildeathc - ~ATH compiler by ruleofsix, version "  TILDEATHC_VERSION "\n");
				exit(EXIT_SUCCESS);
				break;
			case '?':
				printf("%s\n", usage_str);
				exit(EXIT_FAILURE);
				break;
		}
	}
	if (output_filename == NULL) {
		output_filename = output_asm ? default_asm_output_filename : default_output_filename;
	}

	if (echo_output) {
		output_asm = true;
	}
	if (optind < argc - 1) {
		for (int i = optind + 1; i < argc; i++) {
			fprintf(stderr, "Error: unrecognized argument \"%s\"\n", argv[i]);
		}
		if (run) {
			unlink(output_filename);
		}
		exit(EXIT_FAILURE);
	}
	if (optind > argc - 1) {
		fprintf(stderr, "Error: missing input file argument.\n");
		if (run) {
			unlink(output_filename);
		}
		exit(EXIT_FAILURE);
	}

	char* input_filename = argv[optind];
	errno = 0;
	FILE* input_file;
	if (strcmp(input_filename, "-") == 0) {
		input_file = stdin;
	} else {
		input_file = fopen(input_filename, "r");
	}
	if (input_file == NULL) {
		perror("tildeathc: error reading input file");
		if (run) {
			unlink(output_filename);
		}
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
		if (run) {
			unlink(output_filename);
		}
		exit(EXIT_FAILURE);
	}

	FILE* asm_file = stdout;
	asm_filename = util_strcat(util_strdup(tmpdir), "/tildeathc_XXXXXX.s");
	if (!echo_output || run) {
		errno = 0;
		if (!output_asm) {
			int asm_file_d = mkstemps(asm_filename, 2);
			if (asm_file_d == -1) {
				perror("tildeathc: error creating temporary file");
				if (run) {
					unlink(output_filename);
				}
				exit(EXIT_FAILURE);
			}
			errno = 0;
			asm_file = fdopen(asm_file_d, "w");
		} else {
			asm_file = fopen(output_filename, "w");
		}
	}

	signal(SIGINT, unlink_asmfile);

	if (asm_file == NULL) {
		perror("tildeathc: error opening temporary file");
		if (run) {
			unlink(output_filename);
		}
		unlink(asm_filename);
		exit(EXIT_FAILURE);
	}

	generate_assembly(il_tree, input_filename, asm_file);
	
	if (output_asm && !run) {
		if (asm_file != stdout) {
			fclose(asm_file);
		}
		exit(EXIT_SUCCESS);
	}
	fclose(asm_file);
	
	errno = 0;
	int32_t pid = fork();

	if (pid < 0) {
		perror("tildeathc: error attempting to fork process");
		unlink(asm_filename);
		exit(EXIT_FAILURE);
	}
	if (pid == 0) {
		errno = 0;
		execlp("gcc", "gcc", asm_filename, "-L" RUNTIME_LIB_DIR, "-L" SRC_LIB_DIR, "-lobjects", "-lutil", "-o", output_filename, (char*) NULL);
		if (errno == 0) { 
			fprintf(stderr, "tildeathc: failed to execute gcc: unknown error\n");
		} else {
			perror("tildeathc: failed to execute gcc");
		}
		unlink(asm_filename);
		unlink(output_filename);
		exit(EXIT_FAILURE); 
	}

	signal(SIGINT, unlink_outfile);

	int status = 0;
	errno = 0;
	if (waitpid(pid, &status, 0) != pid) {
		if (errno == 0) {
			fprintf(stderr, "tildeathc: unknown error waiting for gcc\n");
		} else {
			perror("tildeathc: error waiting for gcc");
		}
		unlink(asm_filename);
		unlink(output_filename);
		exit(EXIT_FAILURE);
	}

	unlink(asm_filename);
	free(asm_filename);

	if (status != 0) {
		fprintf(stderr, "tildeathc: gcc exited with status code %d\n", status);
		unlink(output_filename);
		exit(EXIT_FAILURE);
	}

	if (!run) {
		exit(EXIT_SUCCESS);
	}

	errno = 0;
	pid = fork();
	if (pid < 0) {
		perror("tildeathc: error attempting to fork process");
		unlink(output_filename);
		exit(EXIT_FAILURE);
	} 

	if (pid == 0) {
		execlp(output_filename, output_filename, (char*) NULL);
		if (errno == 0) {
			fprintf(stderr, "tildeathc: unknown error attempting to execute generated binary\n");
		} else {
			perror("tildeathc: error attempting to execute generated binary");
		}
		unlink(output_filename);
		exit(EXIT_FAILURE);
	}

	status = 0;
	errno = 0;
	if (waitpid(pid, &status, 0) != pid) {
		if (errno == 0) {
			fprintf(stderr, "tildeathc: unknown waitpid error.\n");
		} else {
			perror("tildeathc waitpid error");
		}
		unlink(output_filename);
		exit(EXIT_FAILURE);
	}
	free(output_filename);
	unlink(output_filename);
	exit(EXIT_SUCCESS);
}

void unlink_asmfile() {
	unlink(asm_filename);
}

void unlink_outfile() {
	unlink(output_filename);
}

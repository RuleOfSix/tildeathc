#include <objects.h>
#include <nullcheck.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
	struct tildeath_object* left_half = malloc(sizeof(*left_half));
	MALLOC_NULL_CHECK(left_half);
	*left_half = (struct tildeath_object) {.type=ABSTRACT, .state=ALIVE, .halves.left=NULL, .halves.right=NULL};

	struct tildeath_object* right_half = malloc(sizeof(*right_half));
	MALLOC_NULL_CHECK(right_half);
	*right_half = (struct tildeath_object) {.type=ABSTRACT, .state=ALIVE, .halves.left=NULL, .halves.right=NULL};

	struct tildeath_object* input = malloc(sizeof(*input));
	MALLOC_NULL_CHECK(input);
	*input = (struct tildeath_object) {.type=ABSTRACT, .state=ALIVE, .halves.left=left_half, .halves.right=right_half};

	int output = kill(input);
	if (output != 0) {
		fprintf(stderr, "Error: killing an object returned %d status code, expected 0.\n", output);
		exit(EXIT_FAILURE);
	}
	if(input->state != DEAD) {
		fprintf(stderr, "Error: failed to kill() object.\n");
		exit(EXIT_FAILURE);
	}
	if(input->halves.left->state != DEAD || input->halves.right->state != DEAD) {
		fprintf(stderr, "Error: killing an object failed to kill both halves of the object.\n");
		exit(EXIT_FAILURE);
	}
	free(input);
	printf("Kill object test passed.\n");
	exit(EXIT_SUCCESS);
}

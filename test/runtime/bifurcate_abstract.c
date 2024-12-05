#include <objects.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void) {
	struct tildeath_object input = {.type=ABSTRACT, .state=ALIVE, .halves.left=NULL, .halves.right=NULL};
	struct tildeath_object_halves* halves = bifurcate(&input);
	if (halves != &(input.halves)) {
		fprintf(stderr, "Error: abstract bifurcation returned pointer that does not equal the address of the original objects' halves.\n");
		exit(EXIT_FAILURE);
	}
	if (memcmp(halves->left, halves->right, sizeof(*(halves->left))) != 0) {
		fprintf(stderr, "Error: abstract bifurcation created halves that are not equal.\n");
		exit(EXIT_FAILURE);
	}
	if (halves->left->type != ABSTRACT) {
		fprintf(stderr, "Error: abstract bifurcation created halves not of type ABSTRACT.\n");
		exit(EXIT_FAILURE);
	}
	if (halves->left->state != ALIVE) {
		fprintf(stderr, "Error: abstract bifurcation created dead halves.\n"); 
		exit(EXIT_FAILURE);
	}
	if (halves->left->halves.left != NULL || halves->left->halves.right != NULL) {
		fprintf(stderr, "Errror: abstract bifurcation created halves with at least one non-NULL half.\n");
		exit(EXIT_FAILURE);
	}
	free(halves->left);
	free(halves->right);
	printf("Abstract bifurcation test passed.\n");
	exit(EXIT_SUCCESS);
}

#include <objects.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
	struct tildeath_object input = {.type=UNIVERSE, .state=ALIVE, .halves.left=NULL, .halves.right=NULL};
	struct tildeath_object_halves* output = bifurcate(&input);
	if (output != NULL) {
		fprintf(stderr, "Error: universe bifurcation returned non-NULL value.\n");
		exit(EXIT_FAILURE);
	}
	if (input.halves.left != NULL || input.halves.right != NULL) {
		fprintf(stderr, "Error: universe bifurcation populated at least one of the universe's halves with a non-NULL value.\n");
		exit(EXIT_FAILURE);
	}
	printf("Universe bifurcation test succeeded.\n");
	exit(EXIT_SUCCESS);
}

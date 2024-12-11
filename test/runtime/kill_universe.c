#include <objects.h>
#include <stdlib.h>
#include <stdio.h>

int main(void) {
	struct tildeath_object input = {.type=UNIVERSE, .state=ALIVE, .halves.left=NULL, .halves.right=NULL};
	int output = kill(&input);
	if(output != 1) {
		fprintf(stderr, "Error: killing a universe returns %d status code, expected 1.\n", output);
		exit(EXIT_FAILURE);
	}
	if (input.state == DEAD) {
		fprintf(stderr, "Error: killing a universe incorrectly kills the universe.\n");
		exit(EXIT_FAILURE);
	}
	printf("Kill universe test passed.\n");
	exit(EXIT_SUCCESS);
}

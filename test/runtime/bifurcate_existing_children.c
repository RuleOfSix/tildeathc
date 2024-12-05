#include <objects.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void) {
	struct tildeath_object input = {.type=ABSTRACT, .state=ALIVE,
									.halves.left=(struct tildeath_object[]) {{.type=ABSTRACT, .state=DEAD, .halves.left=NULL, .halves.right=NULL}},
									.halves.right=(struct tildeath_object[]) {{.type=ABSTRACT, .state=ALIVE, .halves.left=NULL, .halves.right=NULL}}};
	struct tildeath_object_halves* output = bifurcate(&input);

	if (output == &(input.halves)) {
		fprintf(stderr, "Error: bifurcation of object with existing children returned pointer to existing halves of object.\n");
		exit(EXIT_FAILURE);
	}
	if (memcmp(input.halves.left, output->left, sizeof(*(input.halves.left))) != 0) {
		fprintf(stderr, "Error: bifurcation of object with existing children returned left half not equal to existing left half.\n");
		exit(EXIT_FAILURE);
	}
	if (memcmp(input.halves.right, output->right, sizeof(*(input.halves.right))) != 0) {
		fprintf(stderr, "Error: bifurcation of object with existing children returned right half not equal to existing right half.\n");
		exit(EXIT_FAILURE);
	}
	printf("Test of bifurcation of objects with existing children passed.\n");
	exit(EXIT_SUCCESS);
}

#include <objects.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void) {
	for (uint32_t i = 0; i < 2; i++) {
		struct tildeath_object* obj = create_object((enum tildeath_object_type) i);	
		if (obj == NULL) {
			fprintf(stderr, "Error: object creation returned null pointer.\n");
			exit(EXIT_FAILURE);
		}
		if (obj->type != i) {
			fprintf(stderr, "Error: object creation returned object with incorrect type.\n");
			exit(EXIT_FAILURE);
		}
		if (obj->state != ALIVE) {
			fprintf(stderr, "Error: object creation returned dead object.\n");
			exit(EXIT_FAILURE);
		}
		if (obj->halves.left != NULL || obj->halves.right != NULL) {
			fprintf(stderr, "Error: object creation returned object with at least one non-NULL child.\n");
			exit(EXIT_FAILURE);
		}
		free(obj);
	}
	printf("Object creation test passed.\n");
	exit(EXIT_SUCCESS);
}

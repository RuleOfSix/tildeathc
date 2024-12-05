#include <objects.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main() {
	struct tildeath_input_object* obj = (struct tildeath_input_object*) create_object(INPUT, "Test string");	
	if (obj == NULL) {
		fprintf(stderr, "Error: input object creation returned null pointer.\n");
		exit(EXIT_FAILURE);
	}
	if (obj->type != INPUT) {
		fprintf(stderr, "Error: input object creation returned object with incorrect type.\n");
		exit(EXIT_FAILURE);
	}
	if (obj->state != ALIVE) {
		fprintf(stderr, "Error: input object creation returned dead object.\n");
		exit(EXIT_FAILURE);
	}
	if (obj->halves.left != NULL || obj->halves.right != NULL) {
		fprintf(stderr, "Error: input object creation returned object with at least one non-NULL child.\n");
		exit(EXIT_FAILURE);
	}
	if (obj->input_length != 11) {
		fprintf(stderr, "Error: input object creation returned object with incorrect input_length value.\n");
		exit(EXIT_FAILURE);
	}
	if (strcmp(obj->input_text, "Test string") != 0) {
		fprintf(stderr, "Error: input object creation returned object with incorrect input_text value.\n");
		exit(EXIT_FAILURE);
	}
	free(obj->input_text);
	free(obj);
	printf("Input object creation test passed.\n");
	exit(EXIT_SUCCESS);
}

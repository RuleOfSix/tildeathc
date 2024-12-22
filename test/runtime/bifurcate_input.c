#include <objects.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int main(void) {
	struct tildeath_input_object input_1 = {.type=INPUT, .state=ALIVE, .halves.left=NULL, .halves.right=NULL, .input_length=11, .input_text="Test string"};
	struct tildeath_object_halves* halves_1 = bifurcate((struct tildeath_object*) &input_1);
	if (halves_1 != &(input_1.halves)) {
		fprintf(stderr, "Error: input bifurcation #1 returned pointer that does not equal the address of the original objects' halves.\n");
		exit(EXIT_FAILURE);
	}
	if (halves_1->right->type != INPUT) {
		fprintf(stderr, "Error: input bifurcation #1 created right half not of type ABSTRACT.\n");
		exit(EXIT_FAILURE);
	}
	if (halves_1->left->state != ALIVE || halves_1->right->state != ALIVE) {
		fprintf(stderr, "Error: input bifurcation #1 incorrectly created dead halves.\n"); 
		exit(EXIT_FAILURE);
	}
	if (halves_1->left->halves.left != NULL || halves_1->left->halves.right != NULL || 
		halves_1->right->halves.left != NULL || halves_1->right->halves.right != NULL) {
		fprintf(stderr, "Error: input bifurcation #1 created halves with at least one non-NULL half.\n");
		exit(EXIT_FAILURE);
	}
	if (((struct tildeath_input_object*)halves_1->right)->input_length != 10) {
		fprintf(stderr,"Error: input bifurcation #1 created right half with incorrect input_length.\n");
		exit(EXIT_FAILURE);
	}
	if (strcmp(((struct tildeath_input_object*)halves_1->right)->input_text, "est string") != 0) {
		fprintf(stderr, "Error: input bifurcation #1 created right half with incorrect input_text: \"%s\", expected \"%s\".\n", ((struct tildeath_input_object*)halves_1->right)->input_text, "est string");
		exit(EXIT_FAILURE);
	}
	free(halves_1->left);
	free(halves_1->right);

	struct tildeath_input_object input_2 = {.type=INPUT, .state=ALIVE, .halves.left=NULL, .halves.right=NULL, .input_length=8, .input_text="0 string"};
	struct tildeath_object_halves* halves_2 = bifurcate((struct tildeath_object*) &input_2);
	if (halves_2 != &(input_2.halves)) {
		fprintf(stderr, "Error: input bifurcation #2 returned pointer that does not equal the address of the original objects' halves.\n");
		exit(EXIT_FAILURE);
	}
	if (halves_2->right->type != INPUT) {
		fprintf(stderr, "Error: input bifurcation #2 created right half not of type INPUT.\n");
		exit(EXIT_FAILURE);
	}
	if (halves_2->right->state != ALIVE) {
		fprintf(stderr, "Error: input bifurcation #2 incorrectly created dead right half.\n"); 
		exit(EXIT_FAILURE);
	}
	if (halves_2->left->state != DEAD) {
		fprintf(stderr, "Error: input bifurcation #2 failed to kill left half.\n");
		exit(EXIT_FAILURE);
	}
	if (halves_2->left->halves.left != NULL || halves_2->left->halves.right != NULL || 
		halves_2->right->halves.left != NULL || halves_2->right->halves.right != NULL) {
		fprintf(stderr, "Error: input bifurcation #2 created halves with at least one non-NULL half.\n");
		exit(EXIT_FAILURE);
	}
	if (((struct tildeath_input_object*)halves_2->right)->input_length != 7) {
		fprintf(stderr,"Error: input bifurcation #2 created right half with incorrect input_length.\n");
		exit(EXIT_FAILURE);
	}
	if (strcmp(((struct tildeath_input_object*)halves_2->right)->input_text, " string") != 0) {
		fprintf(stderr, "Error: input bifurcation #2 created right half with incorrect input_text: \"%s\", expected \"%s\".\n", ((struct tildeath_input_object*)halves_2->right)->input_text, " string");
		exit(EXIT_FAILURE);
	}
	free(halves_2->left);
	free(halves_2->right);

	struct tildeath_input_object input_3 = {.type=INPUT, .state=ALIVE, .halves.left=NULL, .halves.right=NULL, .input_length=2, .input_text="00"};
	struct tildeath_object_halves* halves_3 = bifurcate((struct tildeath_object*) &input_3);
	if (halves_3 != &(input_3.halves)) {
		fprintf(stderr, "Error: input bifurcation #3 returned pointer that does not equal the address of the original objects' halves.\n");
		exit(EXIT_FAILURE);
	}
	if (halves_3->right->type != INPUT) {
		fprintf(stderr, "Error: input bifurcation #3 created right half not of type INPUT.\n");
		exit(EXIT_FAILURE);
	}
	if (halves_3->right->state != ALIVE) {
		fprintf(stderr, "Error: input bifurcation #3 incorrectly created dead right half.\n"); 
		exit(EXIT_FAILURE);
	}
	if (halves_3->left->state != DEAD) {
		fprintf(stderr, "Error: input bifurcation #3 failed to kill left half.\n");
		exit(EXIT_FAILURE);
	}
	if (halves_3->left->halves.left != NULL || halves_3->left->halves.right != NULL || 
		halves_3->right->halves.left != NULL || halves_3->right->halves.right != NULL) {
		fprintf(stderr, "Error: input bifurcation #3 created halves with at least one non-NULL half.\n");
		exit(EXIT_FAILURE);
	}
	if (((struct tildeath_input_object*)halves_3->right)->input_length != 1) {
		fprintf(stderr,"Error: input bifurcation #3 created right half with incorrect input_length.\n");
		exit(EXIT_FAILURE);
	}
	if (strcmp(((struct tildeath_input_object*)halves_3->right)->input_text, "0") != 0) {
		fprintf(stderr, "Error: input bifurcation #3 created right half with incorrect input_text: \"%s\", expected \"%s\".\n", ((struct tildeath_input_object*)halves_3->right)->input_text, "0");
		exit(EXIT_FAILURE);
	}
	free(halves_3->left);
	free(halves_3->right);

	struct tildeath_input_object input_4 = {.type=INPUT, .state=ALIVE, .halves.left=NULL, .halves.right=NULL, .input_length=1, .input_text="0"};
	struct tildeath_object_halves* halves_4 = bifurcate((struct tildeath_object*) &input_4);
	if (halves_4 != &(input_4.halves)) {
		fprintf(stderr, "Error: input bifurcation #4 returned pointer that does not equal the address of the original objects' halves.\n");
		exit(EXIT_FAILURE);
	}
	if (halves_4->right->type != INPUT) {
		fprintf(stderr, "Error: input bifurcation #4 created right half not of type INPUT.\n");
		exit(EXIT_FAILURE);
	}
	if (halves_4->right->state != DEAD) {
		fprintf(stderr, "Error: input bifurcation #4 incorrectly created alive right half.\n"); 
		exit(EXIT_FAILURE);
	}
	if (halves_4->left->state != DEAD) {
		fprintf(stderr, "Error: input bifurcation #4 failed to kill left half.\n");
		exit(EXIT_FAILURE);
	}
	if (halves_4->left->halves.left != NULL || halves_4->left->halves.right != NULL || 
		halves_4->right->halves.left != NULL || halves_4->right->halves.right != NULL) {
		fprintf(stderr, "Error: input bifurcation #4 created halves with at least one non-NULL half.\n");
		exit(EXIT_FAILURE);
	}
	if (((struct tildeath_input_object*)halves_4->right)->input_length != 0) {
		fprintf(stderr,"Error: input bifurcation #4 created right half with incorrect input_length.\n");
		exit(EXIT_FAILURE);
	}
	if (strcmp(((struct tildeath_input_object*)halves_4->right)->input_text, "") != 0) {
		fprintf(stderr, "Error: input bifurcation #4 created right half with non-empty input_text.\n");
		exit(EXIT_FAILURE);
	}
	free(halves_4->left);
	free(halves_4->right);

	printf("Input bifurcation tests passed.\n");
	exit(EXIT_SUCCESS);
}

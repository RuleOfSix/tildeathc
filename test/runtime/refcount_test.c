#include<objects.h>
#include<stdio.h>
#include<stdlib.h>

int main(void) {
	struct tildeath_object* root = create_object(ABSTRACT);
	struct tildeath_object_halves* root_halves = bifurcate(root);
	struct tildeath_object* left = root_halves->left;
	struct tildeath_object* right = root_halves->right;
	enum tildeath_object_state left_before_state = left->state;
	enum tildeath_object_state right_before_state = right->state;

	free_object(root);
	
	// This is a janky way to try to detect an early free; in an ideal world, this test simply segfaults if left or right gets freed
	if (left->state != left_before_state || right->state != right_before_state) {
		fprintf(stderr, "Refcount test failed: objects freed early when parent was freed.\n");
		exit(EXIT_FAILURE);
	}

	printf("Refcount test (probably) passed.\n");
	exit(EXIT_SUCCESS);
}

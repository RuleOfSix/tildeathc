#include <objects.h>
#include <util.h>
#include <nullcheck.h>
#include <stdlib.h>
#include <string.h>

struct tildeath_object universal_dead = {.type=ABSTRACT, .state=DEAD, .halves.left=&universal_dead, .halves.right=&universal_dead};


struct tildeath_object* create_object(enum tildeath_object_type type) {
	if (type == INPUT) {
		struct tildeath_input_object* output = malloc(sizeof(*output));
		MALLOC_NULL_CHECK(output);

		int text_capacity = 10;
		char* input_text = malloc(sizeof(*input_text) * text_capacity);
		MALLOC_NULL_CHECK(input_text);
		int text_length = 0;
		for (char c = getc(stdin); c == '1' || c == '0'; c = getc(stdin)) {
			if (text_length + 1 >= text_capacity) {
				text_capacity *= 2;
				input_text = realloc(input_text, sizeof(*input_text) * text_capacity);
				MALLOC_NULL_CHECK(input_text);
			}
			input_text[text_length] = c;
			text_length++;
		}

		*output = (struct tildeath_input_object){.type=type, 
												 .state=ALIVE,
												 .halves.left=NULL,
												 .halves.right=NULL,
												 .input_length=text_length,
												 .input_text=input_text};
		return (struct tildeath_object*) output;
	} else {
		struct tildeath_object* output = malloc(sizeof(*output));
		MALLOC_NULL_CHECK(output);	
		*output = (struct tildeath_object){.type=type, 
										   .state=ALIVE, 
										   .halves.left=NULL, 
										   .halves.right=NULL};
		return output;
	}
}

struct tildeath_object_halves* bifurcate(struct tildeath_object* object) {
	if (object == &universal_dead || object == NULL) {
		return &(universal_dead.halves);
	}
	if (object->type == UNIVERSE) {
		return NULL;
	}
	if (object->halves.left != NULL && object->halves.right != NULL) {
		struct tildeath_object_halves* output = malloc(sizeof(*output));
		memcpy(output, &(object->halves), sizeof(*output));
		return output;
	} else if(object->halves.left != NULL || object->halves.right != NULL) {
		fprintf(stderr, "Error: malformed tree data.\n");
		exit(EXIT_FAILURE);
	} else {
		if (object->type == INPUT) {
			object->halves.left = malloc(sizeof(struct tildeath_input_object));
			MALLOC_NULL_CHECK(object->halves.left);
			*(object->halves.left) = (struct tildeath_object){.type=INPUT,
													   .state=ALIVE,
													   .halves.left=NULL,
													   .halves.right=NULL};
			if (((struct tildeath_input_object*)object)->input_text[0] == '0') {
				object->halves.left->state = DEAD;
			}
			
			object->halves.right = malloc(sizeof(struct tildeath_input_object));
			MALLOC_NULL_CHECK(object->halves.right);
			*(object->halves.right) = (struct tildeath_object){.type=INPUT,
													   .state=ALIVE,
													   .halves.left=NULL,
													   .halves.right=NULL};
			((struct tildeath_input_object*)object->halves.right)->input_length = ((struct tildeath_input_object*)object)->input_length - 1;
			((struct tildeath_input_object*)object->halves.right)->input_text = util_strdup(((struct tildeath_input_object*)object)->input_text + 1);
			if (((struct tildeath_input_object*)object->halves.right)->input_length == 0) {
				object->halves.right->state = DEAD;
			}
		} else {
			object->halves.left = malloc(sizeof(*(object->halves.left)));
			MALLOC_NULL_CHECK(object->halves.left);
			*(object->halves.left) = (struct tildeath_object){.type=ABSTRACT,
															  .state=ALIVE,
															  .halves.left=NULL,
															  .halves.right=NULL};


			object->halves.right = malloc(sizeof(*(object->halves.right)));
			MALLOC_NULL_CHECK(object->halves.right);
			*(object->halves.right) = (struct tildeath_object){.type=ABSTRACT,
															   .state=ALIVE,
															   .halves.left=NULL,
															   .halves.right=NULL};

		}
		return &(object->halves);
	}
}

int32_t kill(struct tildeath_object* object) {
	if (object->type == UNIVERSE) {
		return 1;
	}
	if (object->state != DEAD) {
		object->state = DEAD;
	}
	free_object(object->halves.left);
	free_object(object->halves.right);
	object->halves.left = &universal_dead;
	object->halves.right = &universal_dead;
	return 0;
}

int32_t free_object(struct tildeath_object* object) {
	if (object == &universal_dead || object == NULL) {
		return 1;
	}
	if (object->type == INPUT) {
		free(((struct tildeath_input_object*) object)->input_text);
	}
	free_object(object->halves.left);
	free_object(object->halves.right);
	free(object);
	return 0;
}


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
												 .refcount=1,
												 .input_length=text_length,
												 .input_text=input_text};
		return (struct tildeath_object*) output;
	} else {
		struct tildeath_object* output = malloc(sizeof(*output));
		MALLOC_NULL_CHECK(output);	
		*output = (struct tildeath_object){.type=type, 
										   .state=ALIVE, 
										   .halves.left=NULL, 
										   .halves.right=NULL, 
										   .refcount=1};
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
		object->halves.left->refcount++;
		object->halves.right->refcount++;
		return &(object->halves);
	} else if(object->halves.left != NULL || object->halves.right != NULL) {
		fprintf(stderr, "~ATH Runtime Error: Cannot bifurcate malformed object.\n");
		exit(EXIT_FAILURE);
	} else {
		if (object->type == INPUT) {
			object->halves.left = malloc(sizeof(struct tildeath_object));
			MALLOC_NULL_CHECK(object->halves.left);
			*(object->halves.left) = (struct tildeath_object){.type=ABSTRACT,
													   .state=ALIVE,
													   .halves.left=NULL,
													   .halves.right=NULL,
													   .refcount=2};
																
			if (((struct tildeath_input_object*)object)->input_text[0] == '0') {
				object->halves.left->state = DEAD;
			}
			
			object->halves.right = malloc(sizeof(struct tildeath_input_object));
			MALLOC_NULL_CHECK(object->halves.right);
			*(object->halves.right) = (struct tildeath_object){.type=INPUT,
													   .state=ALIVE,
													   .halves.left=NULL,
													   .halves.right=NULL,
													   .refcount=2};
			((struct tildeath_input_object*)object->halves.right)->input_length = ((struct tildeath_input_object*)object)->input_length - 1;
			((struct tildeath_input_object*)object->halves.right)->input_text = malloc(((struct tildeath_input_object*)object->halves.right)->input_length * sizeof(char));
			MALLOC_NULL_CHECK(((struct tildeath_input_object*)object->halves.right)->input_text);
			memcpy(((struct tildeath_input_object*)object->halves.right)->input_text, ((struct tildeath_input_object*)object)->input_text + 1, ((struct tildeath_input_object*)object->halves.right)->input_length);
			if (((struct tildeath_input_object*)object->halves.right)->input_length == 0) {
				object->halves.right->state = DEAD;
			}
		} else {
			object->halves.left = malloc(sizeof(*(object->halves.left)));
			MALLOC_NULL_CHECK(object->halves.left);
			*(object->halves.left) = (struct tildeath_object){.type=ABSTRACT,
															  .state=ALIVE,
															  .halves.left=NULL,
															  .halves.right=NULL,
															  .refcount=2};


			object->halves.right = malloc(sizeof(*(object->halves.right)));
			MALLOC_NULL_CHECK(object->halves.right);
			*(object->halves.right) = (struct tildeath_object){.type=ABSTRACT,
															   .state=ALIVE,
															   .halves.left=NULL,
															   .halves.right=NULL,
															   .refcount=2};

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
	if (--(object->refcount) > 0) {
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

void** allocate_progvars(size_t num_vars) {
	void** progvars = calloc(num_vars, sizeof(void*));
	MALLOC_NULL_CHECK(progvars);
	return progvars;
}

void print(char* str) {
	printf("%s", str);
}


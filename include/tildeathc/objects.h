#ifndef TILDEATHC_OBJECTS_H
#define TILDEATHC_OBJECTS_H

#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>

enum tildeath_object_type {
	ABSTRACT,
	UNIVERSE,
	INPUT
};

enum tildeath_object_state {
	ALIVE,
	DEAD
};

struct tildeath_object_halves {
	struct tildeath_object* left;
	struct tildeath_object* right;
};

struct tildeath_object {
	enum tildeath_object_type type;
	enum tildeath_object_state state;
	struct tildeath_object_halves halves;
	int32_t refcount;
};

struct tildeath_input_object {
	enum tildeath_object_type type;
	enum tildeath_object_state state;
	struct tildeath_object_halves halves;
	int32_t refcount;
	int input_length;
	char* input_text;
};


struct tildeath_object* create_object(enum tildeath_object_type type);
struct tildeath_object_halves* bifurcate(struct tildeath_object* object);
int32_t kill(struct tildeath_object* object);
int32_t free_object(struct tildeath_object* object);
void** allocate_progvars(size_t num_vars);
void print(char* str);

#endif

#ifndef TILDEATHC_VALIDATE_IL_H
#define TILDEATHC_VALIDATE_IL_H

#include <stdbool.h>
#include <stdint.h>
struct il_node;

struct varlist {
	int64_t memsize;
	int64_t length;
	char** list;
};

bool validate_il(struct il_node* node, struct varlist* vars);
void free_varlist(struct varlist* vars);

#endif

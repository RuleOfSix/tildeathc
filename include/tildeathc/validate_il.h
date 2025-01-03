#ifndef TILDEATHC_VALIDATE_IL_H
#define TILDEATHC_VALIDATE_IL_H

#include <stdbool.h>
#include <stdint.h>
struct il_node;
struct varlist;

/* The second parameter should always be NULL for external calls */
bool validate_il(struct il_node* node);
void free_varlist(struct varlist* vars);

#endif

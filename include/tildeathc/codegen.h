#ifndef TILDEATHC_CODEGEN_H
#define TILDEATHC_CODEGEN_H

#include <stdio.h>
#include <stdint.h>
struct il_node;

int32_t generate_assembly(const struct il_node* il, char* filename, FILE* output);

#endif

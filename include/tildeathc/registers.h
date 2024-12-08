#ifndef TILDEATHC_REGISTERS_H
#define TILDEATHC_REGISTERS_H

#include <stdint.h>

#define NUM_REGISTERS 6


enum cpu_register {
	R1,
	R2,
	R3,
	R4,
	R5,
	R6,
	ENOREGS
};

enum bit_width {
	B32,
	B64
};

enum cpu_register allocate_register();
int32_t free_register(enum cpu_register reg);
char* register_name(enum cpu_register reg, enum bit_width width);

#endif

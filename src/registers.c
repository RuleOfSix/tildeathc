#include <registers.h>
#include <codegen.h>
#include <util.h>
#include <stdint.h>
#include <stdbool.h>

bool register_array[NUM_REGISTERS] = {0};

enum cpu_register allocate_register(void) {
	for (int32_t i = 0; i < NUM_REGISTERS; i++) {
		if (register_array[i] == false) {
			register_array[i] = true;
			return i;
		}
	}
	return ENOREGS;
}

int32_t free_register(enum cpu_register reg) {
	if (reg == ENOREGS) {
		return 1;
	}
	if (register_array[reg] == false) {
		return 1;
	}
	register_array[reg] = false;
	return 0;
}

char* register_name(enum cpu_register reg, enum bit_width width) {
	switch (reg) {
		case R1:
			return width == B64 ? util_strdup("%rcx") : util_strdup("%ecx");
		case R2:
			return width == B64 ? util_strdup("%rdx") : util_strdup("%edx");
		case R3:
			return width == B64 ? util_strdup("%r8") : util_strdup("%r8d");
		case R4:
			return width == B64 ? util_strdup("%r9") : util_strdup("%r9d");
		case R5:
			return width == B64 ? util_strdup("%r10") : util_strdup("%r10d");
		case R6:
			return width == B64 ? util_strdup("%r11") : util_strdup("%r11d");
		case ENOREGS:
			return NULL;
	}
	return NULL;
}

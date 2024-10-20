#ifndef RULEOFSIX_NULLCHECK_H
#define RULEOFSIX_NULLCHECK_H

#include <stdlib.h>

#define MALLOC_NULL_CHECK(var) do { \
	if ((var) == NULL) { \
		fprintf(stderr, "Memory allocation failure. Terminating.\n"); \
		exit(EXIT_FAILURE); \
	} \
} while(0)
#endif

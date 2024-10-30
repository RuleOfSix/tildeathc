#ifndef RULEOFSIX_NULLCHECK_H
#define RULEOFSIX_NULLCHECK_H

#include <stdio.h>
#include <stdlib.h>

#define MALLOC_NULL_CHECK(var) do { \
	if ((var) == NULL) { \
		fprintf(stderr, "Memory allocation failure at line %d of file %s. Terminating.\n", __LINE__, __FILE__); \
		exit(EXIT_FAILURE); \
	} \
} while(0)

#define ASSERT_NOT_NULL(var) do { \
	if ((var) == NULL) { \
		fprintf(stderr, "Unexpected null pointer at line %d of file %s. Terminating.\n", __LINE__, __FILE__); \
		exit(EXIT_FAILURE); \
	} \
} while(0)
#endif

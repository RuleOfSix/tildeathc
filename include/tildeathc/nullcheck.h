#ifndef RULEOFSIX_NULLCHECK_H
#define RULEOFSIX_NULLCHECK_H

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

#define MALLOC_NULL_CHECK(var) do { \
	if ((var) == NULL) { \
		fprintf(stderr, "Error at %s, line %d: Memory allocation failure: %s. Terminating.\n", __FILE__, __LINE__, strerror(errno)); \
		exit(EXIT_FAILURE); \
	} \
} while(0)

#define ASSERT_NOT_NULL(var) do { \
	if ((var) == NULL) { \
		fprintf(stderr, "Error at %s, line %d: Unexpected null pointer. Terminating.\n", __FILE__, __LINE__); \
		exit(EXIT_FAILURE); \
	} \
} while(0)
#endif

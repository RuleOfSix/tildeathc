#ifndef TILDEATHC_UTIL_H
#define TILDEATHC_UTIL_H

#include <stdint.h>
#include <stdbool.h>

struct strarray {
	char** array;
	int64_t len;
	int64_t cap;
};

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Returns a pointer to a new copy of the given string that must later be freed. *
 * NOT SAFE FOR NON-STRINGS. DO NOT USE WITH UNTRUSTED CHAR BUFFERS.             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
char* util_strdup(const char* str);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Reallocs dest to add src onto the end, guaranteeing null-termination. Src will*
 * be inserted starting where the original null byte in dest was. Returns NULL if*
 * allocation fails or if either src or dest are NULL. NOT SAFE FOR NON-STRINGS. *
 * DO NOT USE WITH UNTRUSTED CHAR BUFFERS.                                       *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
char* util_strcat(char* dest, const char* src);

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Appends new_entry to a.array, increasing the capacity and reallocing if       *
 * necessary. Will allocate a new string with util_strdup if duplicate is true,  *
 * and simply add the new_entry pointer directly to a.array otherwise. Will      *
 * allocate a new array with a capacity of two if a.array is NULL, but fails with*
 * error code 1 if a itself is NULL.                                             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
int32_t append_to_strarray(struct strarray* a, char* new_entry, bool duplicate);

#endif

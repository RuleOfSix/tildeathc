#include <util.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include <nullcheck.h>

char* util_strdup(const char* str) {
	int64_t len = strlen(str);
	char* out = malloc((len + 1) * sizeof(char));
	MALLOC_NULL_CHECK(out);
	memcpy(out, str, len + 1);
	return out;
}

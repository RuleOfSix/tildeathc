#include <util.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>

char* util_newstr(const char* str) {
	int64_t len = strlen(str);
	char* out = malloc((len + 1) * sizeof(char));
	memcpy(out, str, len + 1);
	return out;
}

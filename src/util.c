#include <util.h>
#include <stdint.h>
#include <string.h>
#include <nullcheck.h>

char* util_strdup(const char* str) {
	int64_t len = strlen(str);
	char* out = malloc((len + 1) * sizeof(char));
	MALLOC_NULL_CHECK(out);
	memcpy(out, str, len + 1);
	return out;
}

char* util_strcat(char* dest, const char* src) {
	if (dest == NULL || src == NULL) {
		return NULL;
	}
	size_t dest_len = strlen(dest);
	size_t src_len = strlen(src);
	dest = realloc(dest, (dest_len + src_len + 1) * sizeof(*dest));
	MALLOC_NULL_CHECK(dest);
	
	char* p;
	for (p = dest; *p != '\0'; p++);
	memcpy(p, src, src_len + 1);

	return dest;
}

int32_t append_to_strarray(struct strarray* a, char* new_entry, bool duplicate) {
	if (a == NULL) {
		return 1;
	}
	if (a->array == NULL) {
		a->cap = 2;
		a->len = 0;
		a->array = malloc(a->cap * sizeof(*(a->array)));
		MALLOC_NULL_CHECK(a->array);
	}
	if (a->len >= a->cap) {
		a->cap *= 2;
		a->array = realloc(a->array, a->cap * sizeof(*(a->array)));
		MALLOC_NULL_CHECK(realloc);
	}
	a->array[a->len] = duplicate ? util_strdup(new_entry) : new_entry;
	a->len++;
	return 0;
}

int32_t append_to_ptrarray(struct ptrarray* a, void* new_entry) {
	if (a == NULL) {
		return 1;
	}
	if (a->array == NULL) {
		a->cap = 2;
		a->len = 0;
		a->array = malloc(a->cap * sizeof(*(a->array)));
		MALLOC_NULL_CHECK(a->array);
	}
	if (a->len >= a->cap) {
		a->cap *= 2;
		a->array = realloc(a->array, a->cap * sizeof(*(a->array)));
		MALLOC_NULL_CHECK(realloc);
	}
	a->array[a->len] = new_entry;
	a->len++;
	return 0;
}

struct strarray* dup_strarray(struct strarray* a) {
	struct strarray* output = malloc(sizeof(*output));
	MALLOC_NULL_CHECK(output);
	output->len = a->len;
	output->cap = a->cap;
	output->array = malloc(sizeof(*(output->array)) * output->cap);
	MALLOC_NULL_CHECK(output->array);
	for (int64_t i = 0; i < output->len; i++) {
		output->array[i] = util_strdup(a->array[i]);
	}
	return output;
}

struct ptrarray* dup_ptrarray(struct ptrarray* a) {
	struct ptrarray* output = malloc(sizeof(*output));
	MALLOC_NULL_CHECK(output);
	output->len = a->len;
	output->cap = a->cap;
	output->array = malloc(sizeof(*(output->array)) * output->cap);
	MALLOC_NULL_CHECK(output->array);
	for (int64_t i = 0; i < output->len; i++) {
		output->array[i] = a->array[i];
	}
	return output;
}

void free_strarray(struct strarray* a) {
	for (int64_t i = 0; i < a->len; i++) {
		free(a->array[i]);
	}
	free(a->array);
}

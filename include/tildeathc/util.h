#ifndef TILDEATHC_UTIL_H
#define TILDEATHC_UTIL_H

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Returns a pointer to a new copy of the given string that must later be freed. *
 * NOT SAFE FOR NON-STRINGS. DO NOT USE WITH UNTRUSTED CHAR BUFFERS.             *
 * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * */
char* util_strdup(const char* str);

#endif

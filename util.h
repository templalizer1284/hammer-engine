#ifndef HAMMER_UTIL_H
#define HAMMER_UTIL_H

#include "hammer.h"

/* Filesystem path separator for unix systems. If not, then windows. */
#define SEP "/"

#ifdef _WIN32
#define SEP "\"
#endif

#ifdef _WIN64
#define SEP "\"
#endif

#define ps(x) printf("%s\n", x);

typedef struct h_String {
	char *text;
	size_t size;
} h_String;

h_String h_NewString(const char *);
void h_ConcatString(h_String *, int, ...);
void h_DestroyString(h_String *);
void h_DestroyAllStrings(void);
void h_Vec3Mod(Vector3*, Vector3);

h_String *strings[1000];
int strings_counter;

#endif

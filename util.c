#include "util.h"

void
h_Vec3Mod(Vector3 *vec, Vector3 v) {
	vec->x = v.x;
	vec->y = v.y;
	vec->z = v.z;
}

h_String
h_NewString(const char *s) {
	h_String string;

	const size_t s_size = strlen(s);

	string.text = malloc(sizeof(char) * strlen(s));
	string.text = strdup(s);
	
	string.size = s_size;

	strings[strings_counter] = &string;
	strings_counter++;

	return string;
}

void
h_ConcatString(h_String *destination, int n, ...) {
	
	va_list args;
	va_start(args, n);

	size_t new_destination_size = destination->size + 1;

	for(int i = 0; i < n; i++) {
		new_destination_size += strlen(va_arg(args, const char *));
	}

	va_end(args);

	if( (destination->text = realloc(destination->text, new_destination_size * sizeof(char))) == NULL) {
		perror("Error: Memory cannot be allocated.\n");
		_Exit(1);
	} else {
		va_start(args, n);
		for(int i = 0; i < n; i++) {
			(void)strcat(destination->text, va_arg(args, const char *));
		}
	}

	destination->size = new_destination_size;
	va_end(args);
}

void
h_DestroyString(h_String *string) {
	if(string->text == NULL) {
		perror("String already freed or wasn't used.\n");
		return;
	} else {
		free(string->text);
	}
}

void
h_DestroyAllStrings(void) {
	for(int i = 0; i < strings_counter; i++) {
		if(strings[strings_counter] == NULL) {
			// Pass
		} else {
			free(strings[strings_counter]);
		}
	}
}

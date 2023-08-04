#ifndef H_SHADER
#define H_SHADER

#include "hammer.h"

typedef struct Shader Shader;

struct Shader {
	GLuint vertex;
	GLuint fragment;
	GLuint program;
	char *vertexcode;
	char *fragmentcode;
	void (*Load)(Shader *, const char *, const char *);
	void (*Destroy)(Shader *);
};

Shader ShaderInit(void);
void ShaderLoad(Shader*, const char *, const char *);
void ShaderDestroy(Shader *);

#endif

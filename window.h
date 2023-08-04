#ifndef H_WINDOW
#define H_WINDOW

#include "hammer.h"

typedef struct h_Window h_Window;

struct h_Window {
	SDL_Window *window;
	SDL_GLContext gl_context;
	SDL_Event event;
	int error;
	void (*Destroy)(h_Window*);
};

enum VAO_IDs { TriangleVAO, NumVAOs };
enum VBO_IDs { CubeVBO, ColorVBO, TriangleVBO, NumVBOs};
enum Attribute_IDs { vModelPosition = 0, vColor, vTriangle };
enum ModelID { Cube, NumModels };

static GLuint VAO[NumVAOs];

h_Window h_WindowInit();
void h_WindowDestroy(h_Window*);

#endif

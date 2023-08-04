#include "window.h"

h_Window
h_WindowInit() {
	static h_Window window;

	window.Destroy = &h_WindowDestroy;

	h_Config config = h_ConfigInit();
	
	if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS) != 0){
		printf("SDL_Init error: %s\n", SDL_GetError());
		goto END_ERROR;
	}

	SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
	SDL_GL_SetAttribute( SDL_GL_ACCELERATED_VISUAL, 1 );
	SDL_GL_SetAttribute( SDL_GL_RED_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_GREEN_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_BLUE_SIZE, 8 );
	SDL_GL_SetAttribute( SDL_GL_ALPHA_SIZE, 8 );

	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MAJOR_VERSION, 4 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_MINOR_VERSION, 6 );
	SDL_GL_SetAttribute( SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE );

	int window_mode = SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL;
	if(config.mode == 1) {
		window_mode = SDL_WINDOW_FULLSCREEN | SDL_WINDOW_OPENGL;
	}
	
	if( ( window.window = SDL_CreateWindow(PROP_GameName,
					       config.xpos,
					       config.ypos,
					       config.height,
					       config.width,
					       window_mode)) == NULL ){
		printf("SDL_CreateWindow error: %s\n", SDL_GetError());
	}

	if( ( window.gl_context = SDL_GL_CreateContext(window.window) ) == NULL ){
		printf("SDL_CreateContext error: %s\n", SDL_GetError());
	}

	/* glEnable(GL_DEPTH_TEST);
	 * glDepthFunc(GL_LESS); */

	glClearColor(0.0f, 0.0f, 0.4f, 0.0f);

	glGenVertexArrays(NumVAOs, VAO);
	glBindVertexArray(VAO[TriangleVAO]);

	return window;
	
END_ERROR:
	window.error = 1;
	return window;
}

void
h_WindowDestroy(h_Window *window) {
	SDL_GL_DeleteContext(window->gl_context);
	SDL_DestroyWindow(window->window);
	SDL_Quit();
}

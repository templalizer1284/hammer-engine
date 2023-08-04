#include "hammer.h"

int
main(void){
	h_Window window = h_WindowInit();

	if(window.error > 0) {
		fprintf(stderr, "Window initialization failed.\n Aborting.\n");
		goto END;
	}
	
	Shader shader = ShaderInit();
	shader.Load(&shader, "VertexShader.glsl", "FragmentShader.glsl");
	glUseProgram(shader.program);

	h_Model cube = h_ModelInit("t.obj");
	
 	while(true){
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		if(SDL_PollEvent(&window.event) != 0){
			if(window.event.key.keysym.sym == SDLK_q){
				break;
			}
		}

		cube.shaderID = vModelPosition;
		cube.Render(&cube);
		
		SDL_GL_SwapWindow(window.window);
		SDL_Delay(20);
	}
	
	cube.Destroy(&cube);
	glDeleteVertexArrays(NumVAOs, VAO);

	window.Destroy(&window);
END:
	return 0;
}

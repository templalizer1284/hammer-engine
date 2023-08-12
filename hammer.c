#include "hammer.h"

int
main(/* int argc, char *argv[] */){

	/* if(argc == 1) {
	 * 	fprintf(stderr, "Error, please specify game folder with -game argument.\n");
	 * } */

	/* if(strcmp(argv[1], "-game")) {
	 * 	if(strcmp(argv[2], "dry")) {
	 * 		fprintf(stdout, "Dry run for dev purposes, game base not selected.\n");
	 * 	}
	 * 
	 * 	FILE *fp = fopen(argv[2], "r");
	 * 	
	 * 	if(fp == NULL) {
	 * 		fprintf(stderr, "Cannot open %s, directory doesn't exist.\n", argv[2]);
	 * 		fclose(fp);
	 * 		return 1;
	 * 	}
	 * 
	 * 	else {
	 * 		// Load files.
	 * 		fprintf(stdout, "Found %s, Loading...\n", argv[2]);
	 * 		fclose(fp);
	 * 	}
	 * }
	 * 
	 * else {
	 * 	fprintf(stderr, "Error, unrecognized option %s.\n", argv[1]);
	 * 	return 1;
	 * } */
	
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
	
	//cube.Destroy(&cube);
	glDeleteVertexArrays(NumVAOs, VAO);

	window.Destroy(&window);
END:
	return 0;
}

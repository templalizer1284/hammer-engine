#include "window.h"

h_Window
h_WindowInit(void) {
	h_Window window;

	window.Destroy = &h_WindowDestroy;

	/* Initialize and read config file. */
	h_Config config = h_ConfigInit();

	/* Raylib logging only display warnings and errors. */
	SetTraceLogLevel(LOG_WARNING);
	
	InitWindow(config.width, config.height, config.game_name);

	/* Game will run at 30 FPS. */
	SetTargetFPS(30);
	
	return window;
}

void
h_WindowDestroy(void) {
	CloseWindow();
}

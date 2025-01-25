#include "processor.h"

void
h_MainLoop(void) {

	debug = false; // Do not display debugging stuff unless specified otherwise in base root cfg.

	/* Initialize window */
	/* Before window initializes it checks every config file for syntax and resources error.
	 * If everything is okay then window is renderer. */
	h_Window window = h_WindowInit();

	/* Set up camera perspective and coordinates */
	Camera camera = { 0 };
	camera.position = (Vector3){ 0.0f, 5.0f, -7.0f };
	camera.target = (Vector3){ 0.0f, 0.0f, 0.0f };
	camera.up = (Vector3){ 0.0f, 1.0f, 0.0f };
	camera.fovy = 45.0f;
	camera.projection = CAMERA_PERSPECTIVE;

	/* Loading resources */

	/* Loading hero model first, every game is obligated to have hero and map models as basis for level. */
	/* In Models array, 0 is always hero and 1 is always map. ALWAYS, do not change this. */
	Models[0] = h_ModelInit(current_level->hero, false);
	model_counter++;

	/* Loading map model */
	Models[1] = h_ModelInit(current_level->map, false);
	model_counter++;

	/* Loading entities */
	if(current_level->entity_count == 0) {
		// pass, no entities
	} else {	
		for(int i = 0; i < current_level->entity_count; i++) {
			Models[model_counter] = h_ModelInit(current_level->entities[i], true);
			model_counter++;
		}
	}
	
	printf("Entities loaded: %d\n", current_level->entity_count);

	/* Parsing logic */
	h_ConfigParseLogic();

	while(!WindowShouldClose()) {
		BeginDrawing();
		
		if(debug) {
			DrawFPS(10,10);
		}
		
		ClearBackground(DARKPURPLE);
		BeginMode3D(camera);

		if(debug) {
			DrawGrid(100, 1.0f);
		}

		/* Rendering models */
		for(int i = 0; i < model_counter; i++) {
			if(Models[i].render == true) {
				h_ModelDraw(&Models[i]);
			}

			// Rendering collision boxes, debugging
			if(debug) {
				for(int i = 0; i < model_counter; i++) {
					DrawBoundingBox(Models[i].transformedBox, WHITE );
				}
			}
		}

		/* Event checking Section */
		/* Input */

		// Checking HERO input
		if(IsKeyDown(default_controls->sprint)) {
			default_controls->current_speed = default_controls->speed + default_controls->sprint_factor;		
		} else {
			default_controls->current_speed = default_controls->speed;
		}

		if(IsKeyDown(default_controls->forward)) {
			Models[0].position.x += default_controls->current_speed * sin(DEG2RAD * Models[0].angle);
			Models[0].position.z += default_controls->current_speed * cos(DEG2RAD * Models[0].angle);
		}

		if(IsKeyDown(default_controls->backwards)) {
			Models[0].position.x -= default_controls->current_speed * sin(DEG2RAD * Models[0].angle);
			Models[0].position.z -= default_controls->current_speed * cos(DEG2RAD * Models[0].angle);
		}

		if(IsKeyDown(default_controls->strafe_left)) {
			Models[0].position.x += default_controls->current_speed * sin(DEG2RAD * (Models[0].angle + 90.0f));
			Models[0].position.z += default_controls->current_speed * cos(DEG2RAD * (Models[0].angle + 90.0f));
		}

		if(IsKeyDown(default_controls->strafe_right)) {
			Models[0].position.x += default_controls->current_speed * sin(DEG2RAD * (Models[0].angle - 90.0f));
			Models[0].position.z += default_controls->current_speed * cos(DEG2RAD * (Models[0].angle - 90.0f));
		}

		if(IsKeyDown(default_controls->turn_left)) {
			Models[0].angle += default_controls->turn_speed;
			Matrix rotation = MatrixRotateY(DEG2RAD * Models[0].angle);
			Models[0].model.transform = rotation;
		}

		if(IsKeyDown(default_controls->turn_right)) {
			Models[0].angle -= default_controls->turn_speed;
			Matrix rotation = MatrixRotateY(DEG2RAD * Models[0].angle);
			Models[0].model.transform = rotation;
		}

		
		/* Collisions */
		if(collisions_counter > -1) {

			// Go through array to find out wanted events
			for(int i = 0; i < collisions_counter; i++) {

				// When found check if they collide
				if(CheckCollisionBoxes( collisions[i].first_model->transformedBox, collisions[i].second_model->transformedBox )) {
					// Check for actions
					switch(collisions[i].action) {
						case H_PRINT:
							printf("%s\n", collisions[i].text_data);
						break;
					}
				}
			}
		}
		
		EndDrawing();
		EndMode3D();
	}

	/* Destroying resources */

	/* Models */
	for(int i = 0; i < model_counter; i++) {
		h_ModelDestroy(&Models[i]);
	}

	window.Destroy();
}

#include "config.h"

h_Config
h_ConfigInit(void) {
	
	FILE *fp = fopen("hammercfg", "r");

	bool base = false;

	/* If file doesn't exist create new one with default settings. */
	if(access("hammercfg", F_OK) != 0) {
		
		FILE *fc = fopen("hammercfg", "a+");
		fprintf(fc, "window height 800\n");
		fprintf(fc, "window width 600\n");

		/* 0 - windowed */
		/* 1 - fullscreen */
		fprintf(fc, "window mode 0");

		cfg.height = 800;
		cfg.width = 600;
		cfg.mode = 0;

		fclose(fc);
	}

	/* If file already exists then just parse settings and return it. */
	else {
		char component[20], property[20];
		int value;
		
		while(fscanf(fp, "%s %s %d", component, property, &value) != EOF) {
			if(strcmp(component, "window") == 0) {
				if(strcmp(property, "height") == 0) {
					cfg.height = value;
				}

				if(strcmp(property, "width") == 0) {
					cfg.width = value;
				}

				if(strcmp(property, "mode") == 0) {
					cfg.mode = value;
				}
			}

			/* Attempt to read game base, only one entry of base is required. Everything else is pulled from base's root config. */
			if(strcmp(component, "base") == 0) {
				/* Taking path for base. */
				(void)strcpy(cfg.base, property);

				base = true;
				
				/* Parse root cfg */
				h_ConfigParseBase(&cfg);
			}
		}

		if(base == false) {
			perror("Base files not present or corrupted.\n");
			_Exit(1);
		}

		fclose(fp);
	}

	return cfg;
}

void
h_ConfigParseBase(h_Config *cfg) {

	FILE *fp = NULL, *fpp = NULL;
	
	/* First, parsing root config. */
	char root_config[30] = "";
	(void)strcat(root_config, cfg->base);
	(void)strcat(root_config, SEP);
	(void)strcat(root_config, "root.cfg");
		
	printf("PATH is %s\n", root_config);

	if( access(root_config, F_OK) != 0 ) {
		perror("Error: Root config file is not present, base folder corrupted.\n");
		_Exit(1);
	}

	else {

		fp = fopen(root_config, "r");
		
		printf("Found base: %s\n", cfg->base);

		char tmp[50];

		while( (fscanf(fp, "%s", tmp)) != EOF ) {
	
			if(strcmp(tmp, "NAME") == 0) {
				fgets(cfg->game_name, sizeof(char) * 32, fp);
			}

			if(strcmp(tmp, "DEBUG") == 0) {
				debug = true;
			}

			/* Parsing starting level */
			if(strcmp(tmp, "START") == 0) {
				fscanf(fp, "%s", tmp);

				char level_cfg[100] = "";
				(void)strcat(level_cfg, cfg->base);
				(void)strcat(level_cfg, SEP);
				(void)strcat(level_cfg, "levels");
				(void)strcat(level_cfg, SEP);
				(void)strcat(level_cfg, tmp);
				(void)strcat(level_cfg, SEP);
				(void)strcat(level_cfg, "cfg.resources");
	
				if( access(level_cfg, F_OK) != 0 ) {
					perror("Level doesn't exist, aborting.\n");
					_Exit(1);
				} else {

					fpp = fopen(level_cfg , "r");
					printf("Starting level '%s'.\n", tmp);

					/* Parsing level stuff */
					h_ConfigParseLevel(fpp);
				}
			}
		}
	}

	fclose(fp);
	fclose(fpp);
}

/* This function reads settings and assets for one particular level. */
void
h_ConfigParseLevel(FILE *fp) {

	char tmp[50];

	static h_Level level;

	int entity_count = 0;

	bool name = false, hero = false, map = false;

	while(fscanf(fp, "%s", tmp) != EOF) {
		if(strcmp(tmp, "NAME") == 0) {
			name = true;
			fscanf(fp, "%s", tmp);
			(void)strcat(level.level_name, tmp);
		}

		if(strcmp(tmp, "HERO") == 0) {
			fscanf(fp, "%s", tmp);

			(void)strcat(level.hero, cfg.base);
			(void)strcat(level.hero, SEP);
			(void)strcat(level.hero, "media/");
			(void)strcat(level.hero, tmp);
			
			if(access(level.hero, F_OK) == 0 ) {
				// pass ok
			} else {
				perror("Hero file doesn't exist.\n");
				_Exit(1);
			}
			
			hero = true;
		}

		if(strcmp(tmp, "MAP") == 0) {
			fscanf(fp, "%s", tmp);

			(void)strcat(level.map, cfg.base);
			(void)strcat(level.map, SEP);
			(void)strcat(level.map, "media");
			(void)strcat(level.map, SEP);
			(void)strcat(level.map, tmp);
			
			if(access(level.map, F_OK) == 0) {
				// pass ok
			} else {
				perror(strcat(tmp, " file doesn't exist.\n"));
				_Exit(1);
			}

			map = true;
		}
		
		if(strcmp(tmp, "ENTITY") == 0) {
			/* Looking for type now */
			fscanf(fp, "%s", tmp);

			if(strcmp(tmp, "STATIC") == 0) {
				/* Read the file */
				fscanf(fp, "%s", tmp);

				h_String entity_path = h_NewString(cfg.base);
				h_ConcatString(&entity_path, 4, SEP, "media", SEP, tmp);

				if(access(entity_path.text, F_OK) == 0) {
					// pass ok
					(void)strcpy(level.entities[entity_count], entity_path.text);
					entity_count++;
				} else {
					perror("Entity file doesn't exist.\n");
					_Exit(1);
				}
			} else {
				perror("Type for entity is not recognized.\n");
				_Exit(1);
			}
		}
	}

	if( name != true && hero != true && map != true ) {
		perror("Essential properties are not present in level.\n");
		perror("Check for NAME, HERO and MAP properties.\n");
		_Exit(1);
	}


	level.entity_count = entity_count;
	current_level = &level;
}

void
h_ConfigParseLogic(void) {
	FILE *fp = NULL;

	char tmp[50];
	collisions_counter = 0;

	// Initialize default controls.
	static h_Control c = {
		0.0f, 0.07f, 0.13f, KEY_W,
		KEY_S, KEY_LEFT, KEY_RIGHT, KEY_A,
		KEY_D, KEY_LEFT_SHIFT, 2.1f
	};

	default_controls = &c;

	h_String log_path = h_NewString(cfg.base);
	h_ConcatString(&log_path, 6, SEP, "levels", SEP, current_level->level_name, SEP, "cfg.logic");

	if(access(log_path.text, F_OK) != 0) {
		perror("Cannot open config logic file, base corrupted.\n");
		_Exit(1);
	}
	
	fp = fopen(log_path.text, "r");
	while(fscanf(fp, "%s", tmp) != EOF) {
	
		if(strcmp(tmp, "POSITION") == 0) {

			fscanf(fp, "%s", tmp);
		
			float x, y, z;		
			fscanf(fp, "%f %f %f", &x, &y, &z);

			for(int i = 0; i < model_counter; i++) {
				if(strcmp(Models[i].name, tmp) == 0) {
					h_Vec3Mod(&Models[i].position, (Vector3) { x, y, z });
				}
			}
		}

		// Checking for collision logic
		// SYNTAX: COLLISION first_model second_model ACTION args(optional)
		if(strcmp(tmp, "COLLISION") == 0) {

			// Get next word
			fscanf(fp, "%s", tmp);

			// Find first model
			for(int i = 0; i < model_counter; i++) {

				// When found, place in array.
				if(strcmp(Models[i].name, tmp) == 0) {
					collisions[collisions_counter].first_model = &Models[i];
				}
			}

			// Get next word
			fscanf(fp, "%s", tmp);

			// Find second model
			for(int i = 0; i < model_counter; i++) {

				// When found, place in array.
				if(strcmp(Models[i].name, tmp) == 0) {
					collisions[collisions_counter].second_model = &Models[i];
				}
			}

			// Get next word
			fscanf(fp, "%s", tmp);

			// Check if action is valid and take arguments if needed.
			if(strcmp(tmp, "PRINT") == 0) {

				// Place it in array
				collisions[collisions_counter].action = H_PRINT;
				
				// For print, only one arg, char * for message.
				fscanf(fp, "%s", tmp);

				(void)strcat(collisions[collisions_counter].text_data, tmp);
			}

			collisions_counter++;
		}
	}

	h_DestroyString(&log_path);
	fclose(fp);
}

/* To be implemented, to be used with in game settings. */
void
h_ConfigWrite(void) {
		
}

void
h_LevelDestroy(h_Level *l) {
	free(l->level_name);
	free(l->hero);
}

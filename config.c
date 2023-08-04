#include "config.h"

h_Config
h_ConfigInit() {
	static h_Config cfg;

	if(cfg.height > 0) {
		return cfg;
	}

	FILE *fp = fopen("hammercfg", "r");

	/* If file doesn't exist create new one with default settings and load it. */
	if(fp == NULL) {
		
		FILE *fc = fopen("hammercfg", "a+");
		fprintf(fc, "window height 800\n");
		fprintf(fc, "window width 600\n");
		fprintf(fc, "window xpos 0\n");
		fprintf(fc, "window ypos 0\n");

		/* 0 - windowed */
		/* 1 - fullscreen */
		fprintf(fc, "window mode 0");

		fclose(fc);
	}

	/* If file already exist then just parse settings and return it. */
	else {
		char *component = malloc(sizeof(char) * 20);
		char *property = malloc(sizeof(char) * 20);
		int *value = malloc(sizeof(int) * 10);

		while(fscanf(fp, "%s %s %d", component, property, value) != EOF) {
			if(strcmp(component, "window") == 0) {
				if(strcmp(property, "height") == 0) {
					cfg.height = *value;
				}

				if(strcmp(property, "width") == 0) {
					cfg.width = *value;
				}

				if(strcmp(property, "xpos") == 0) {
					cfg.xpos = *value;
				}

				if(strcmp(property, "ypos") == 0) {
					cfg.ypos = *value;
				}

				if(strcmp(property, "mode") == 0) {
					cfg.mode = *value;
				}
			}
		}

		free(value);
		free(property);
		free(component);
	}

	return cfg;
}

int
h_Config_Write() {
	return 0;
}

#ifndef HAMMER_CONFIG_H
#define HAMMER_CONFIG_H

#include "hammer.h"

#define PROP_GameName "The Shadowthrone"

typedef struct h_Config h_Config;

struct h_Config {
	int height, width, xpos, ypos, mode;
};

h_Config h_ConfigInit();

#endif

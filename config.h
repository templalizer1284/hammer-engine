#ifndef HAMMER_CONFIG_H
#define HAMMER_CONFIG_H

#include "hammer.h"
#include "util.h"

enum COLLISION_ACTIONS {

	/* 1 arg, char *	
	 * Prints message to stdout.
	 *
	 * Example: 
	 */
	H_PRINT
};

typedef struct h_Config {
	int width, height, mode;
	char base[100];
	char game_name[100];
} h_Config;

typedef struct h_Level {
	int entity_count;
	char level_name[50], hero[50], map[50];
	char entities[1000][100];
} h_Level;

typedef struct h_Control {
	float current_speed;
	float speed;
	float sprint_factor;
	int forward;
	int backwards;
	int turn_left;
	int turn_right;
	int strafe_left;
	int strafe_right;
	int sprint;
	float turn_speed;
} h_Control;

typedef struct h_Collisions {
	struct h_Model *first_model;
	struct h_Model *second_model;
	uint8_t action;
	void *arg1;
	void *arg2;
	char text_data[100];
} h_Collisions;

h_Level *restrict current_level; /* Pointer to a level that player has interaction in real time. */
static char base[20] = "";
bool debug;
h_Control *restrict default_controls;

// Up to 50 collision detections for now.
h_Collisions collisions[50];
int collisions_counter;

h_Config h_ConfigInit(void);
void h_ConfigParseBase(h_Config *);
void h_ConfigParseLevel(FILE *, h_Config *);
void h_ConfigParseLogic(void);
void h_ConfigWrite(void);
void h_LevelDestroy(h_Level *);

#endif

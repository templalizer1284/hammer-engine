#ifndef HAMMER_MODEL_H
#define HAMMER_MODEL_H

#include "hammer.h"

typedef struct h_Model {
	Model model;

	ModelAnimation *animation;
	int animCount;
	int currentAnim;
	bool animate; /* Even if model containts animation we can choose not to animate */

	BoundingBox box, transformedBox;
	Vector3 center; // Center of bounding box, used for calculations

	Vector3 position;
	float angle; /* Rotation */
	bool render; /* If false, loop will skip rendering this model. Default is true. */

	char *restrict file;
	char *restrict name; /* name is used for identification within logic system */

	float scale;
	Color tint;
} h_Model;

h_Model Models[1000];
int model_counter;
bool debug;

h_Model h_ModelInit(char *, bool);
void h_ModelDraw(h_Model *);
void h_ModelDestroy(h_Model *);
BoundingBox CombineBoundingBoxes(BoundingBox, BoundingBox);
Vector3 GetBoundingBoxCenter(BoundingBox);

#endif

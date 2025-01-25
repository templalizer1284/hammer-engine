#ifndef HAMMER_WINDOW_H
#define HAMMER_WINDOW_H

#include "hammer.h"

typedef struct h_Window h_Window;

struct h_Window {
	void (*Destroy)(void);
};

h_Window h_WindowInit(void);
void h_WindowDestroy(void);

#endif

#ifndef H_HAMMER
#define H_HAMMER

/* --------------
 * Core libraries
 * -------------- */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

/* ----------------
 * Third-party code
 * ---------------- */

/* Portable multimedia library */
#include <SDL2/SDL.h>

/* OpenGL stuff */
#include <epoxy/gl.h>
#include <epoxy/glx.h>

/* Math for OpenGL */
#include <cglm/cglm.h>

/* ------------------------
 * Hammer engine components
 * ------------------------ */

/* Configuration utility */
#include "config.h"

/* Window types and functions */
#include "window.h"

/* Shader utilities */
#include "shader.h"

/* Model types and functions */
#include "model.h"

/* Various helping utilities */
#include "util.h"

#endif

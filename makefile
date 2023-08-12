CC = cc
FILES = hammer.c model.c shader.c config.c window.c util.c
INC = -I/usr/local/include -I/usr/X11R6/include
LIBS = -L/usr/local/lib -L/usr/X11R6/lib
LINK = -lSDL2 -lepoxy -lm
FLAGS = -O0 -g -std=c2x -pedantic -Wall -Wextra -DMALLOC_STATS
RELEASE_FLAGS = -O2
EXE = hammer_debug
RELEASE_EXE = hammer
DEBUGGER = lldb

compile:
	$(CC) $(FLAGS) $(INC) $(LIBS) $(LINK) $(FILES) -o $(EXE)

debug:
	MESA_GL_VERSION_OVERRIDE=4.6 MESA_GLSL_VERSION_OVERRIDE=460 $(DEBUGGER) ./$(EXE)

run:
	MALLOC_OPTIONS=D MESA_GL_VERSION_OVERRIDE=4.6 MESA_GLSL_VERSION_OVERRIDE=460 ./$(EXE)

release:
	$(CC) $(RELEASE_FLAGS) $(INC) $(LIBS) $(LINK) $(FILES) -o $(RELEASE_EXE)

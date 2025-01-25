CC = clang
FILES = hammer.c config.c window.c util.c model.c processor.c audio.c

INC = -I/opt/local/include
LIBS = -L/opt/local/lib
LINK = -lraylib

FLAGS = -g -std=c99 -pedantic -Wall -Wextra -fsanitize=address
RELEASE_FLAGS = -O3

EXE = hammer_debug
RELEASE_EXE = hammer
DEBUGGER = lldb
MEM = leaks
MEM_ARGS = --atExit --

# Pattern rule for compiling object files
compile:
	$(CC) $(FLAGS) $(INC) $(LIBS) $(LINK) $(FILES) -o $(EXE)

release: $(OBJS)
	$(CC) $(RELEASE_FLAGS) $(OBJS) $(LIBS) $(LINK) -o $(RELEASE_EXE)

debug:
	$(DEBUGGER) ./$(EXE)

mem:
	$(MEM) $(MEM_ARGS) ./$(EXE)

run: $(EXE)
	./$(EXE)

# Cleanup target
clean:
	rm -f $(EXE)

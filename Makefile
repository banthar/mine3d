
OBJECTS:=error.o actor.o main.o  noise.o utils.o world.o block.o
HEADERS:=actor.h bool.h error.h math.h noise.h utils.h world.h block.h

PACKAGES:=sdl glew sdl-image gl

LIBS:=-lm -lpthread `pkg-config --libs $(PACKAGES)`
CFLAGS:=-std=gnu99 -Wall -march=native -ffast-math `pkg-config --cflags $(PACKAGES)`

CC:=gcc-4.6

main: $(OBJECTS) $(HEADERS) Makefile
		$(CC) $(LIBS) $(OBJECTS) -o main

$(OBJECTS): %.o: %.c Makefile $(HEADERS)
		$(CC) $(CFLAGS) -c $< -o $@

run: main
		./main

debug: main
		gdb ./main --eval-command="run"

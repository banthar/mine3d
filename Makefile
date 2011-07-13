
OBJECTS:=error.o actor.o main.o  noise.o utils.o world.o block.o
HEADERS:=actor.h error.h math.h noise.h utils.h world.h block.h

PACKAGES:=sdl glew sdl-image gl

LIBS:=-lm `pkg-config --libs $(PACKAGES)`
CFLAGS:=-std=c99 -O3 -Wall -march=native -ffast-math `pkg-config --cflags $(PACKAGES)`
LDFLAGS:=

CC:=gcc-4.6
LD:=gcc-4.6

.PHONY: run debug clean

main: $(OBJECTS) $(HEADERS) Makefile
	$(LD) $(LDFLAGS) $(LIBS) $(OBJECTS) -o main

$(OBJECTS): %.o: %.c Makefile $(HEADERS)
	$(CC) $(CFLAGS) -c $< -o $@

clean: 
	rm -f $(OBJECTS)

run: main
	./main

debug: main
	gdb ./main --eval-command="run"

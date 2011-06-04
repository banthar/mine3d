
OBJECTS:=error.o  main.o  noise.o utils.o  world.o
HEADERS:=bool.h error.h  math.h  noise.h  utils.h  world.h

PACKAGES:=sdl glew sdl-image gl

LIBS:=-lm -lpthread `pkg-config --libs $(PACKAGES)`
CFLAGS:=-std=gnu99 -Wall -march=native -ffast-math `pkg-config --cflags $(PACKAGES)`

CC:=gcc-4.6
#CC:=llvm-clang

main: $(OBJECTS) $(HEADERS) Makefile
		$(CC) $(LIBS) $(OBJECTS) -o main

$(OBJECTS): %.o: %.c Makefile $(HEADERS)
		echo $(CC) $< -o $@

run: main
		./main

debug: main
		gdb ./main --eval-command="run"


SOURCES:=*.c
HEADERS:=*.h

PACKAGES:=sdl glew

LIBS:=-lGL -lm -lSDL_image -lpthread
CFLAGS:=-std=gnu99 -Wall  -ffast-math -g

CC:=gcc-4.6
#CC:=llvm-clang

main: $(SOURCES) $(HEADERS) Makefile
		$(CC) `pkg-config --libs --cflags $(PACKAGES)` $(CFLAGS) $(LIBS) $(SOURCES) -o main

run: main
		./main

debug: main
		gdb ./main --eval-command="run"

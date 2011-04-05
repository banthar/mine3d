
SOURCES:=*.c
HEADERS:=*.h

PACKAGES:=sdl glew

LIBS:=-lGL -lm -lSDL_image
CFLAGS:=-std=c99 -O3 -Wall

CC=gcc-4.6

main: $(SOURCES) $(HEADERS) Makefile
		$(CC) `pkg-config --libs --cflags $(PACKAGES)` $(CFLAGS) $(LIBS) $(SOURCES) -o main

run: main
		./main

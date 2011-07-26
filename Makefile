
OBJECTS:=actor.o client.o  noise.o utils.o world.o block.o network.o socket.o worldgen.o
HEADERS:=actor.h block.h config.h math.h network.h noise.h socket.h utils.h worldgen.h world.h

PACKAGES:=sdl glew sdl-image gl ftgl

CFLAGS:=-std=c99 `pkg-config --cflags $(PACKAGES)`
LDFLAGS:=-lm `pkg-config --libs $(PACKAGES)` -lz

CFLAGS+=-g -Wall -DDEBUG 
LDFLAGS+=-g -lstd

# CFLAGS+=-O3 -flto -Werror -Wall
# LDFLAGS+=-flto

.PHONY: run gdb debug clean

debug: client server

release: clean client server

client: $(OBJECTS)

server: actor.o server.o noise.o utils.o world.o block.o network.o socket.o worldgen.o

$(OBJECTS): Makefile $(HEADERS)

clean: 
	rm -f $(OBJECTS)

run: client
	./client

gdb: client
	gdb ./client --eval-command="run"

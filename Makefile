
PACKAGES:=sdl glew sdl-image gl ftgl

CFLAGS:=-std=c1x -fplan9-extensions `pkg-config --cflags $(PACKAGES)`
LDFLAGS:=-lm `pkg-config --libs $(PACKAGES)` -lz

CFLAGS+=-g -Wall -DDEBUG
LDFLAGS+=-g -lstd

# CFLAGS+=-O3 -flto -Werror -Wall
# LDFLAGS+=-flto

.PHONY: run gdb debug clean

debug: client server

release: clean client server

client: actor.o player.o client.o utils.o world.o block.o network.o socket.o noise.o

server: actor.o player.o server.o utils.o noise.o world.o block.o network.o socket.o worldgen.o

*.o: Makefile *.h

clean:
	rm -f $(OBJECTS)

run: client
	./client

gdb: client
	gdb ./client --eval-command="run"

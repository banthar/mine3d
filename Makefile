
PACKAGES:=sdl glew gl ftgl

CFLAGS:=-std=c1x -fplan9-extensions `pkg-config --cflags $(PACKAGES)`
LDFLAGS:=-lm `pkg-config --libs $(PACKAGES)` -lz -lSDL_image

debug: CFLAGS+=-g -Wall -DDEBUG
debug: LDFLAGS+=-g -lstd

release: CFLAGS+=-O3 -flto -Werror -Wall
release: LDFLAGS+=-flto

.PHONY: run gdb debug clean

debug: client server model_view

release: clean client server

client: actor.o player.o client.o utils.o world.o block.o network.o socket.o noise.o worldgfx.o model.o gui.o ui.o

server: actor.o player.o server.o utils.o noise.o world.o block.o network.o socket.o worldgen.o model.o

model_view: model_view.o model.o utils.o

*.o: Makefile *.h

clean:
	rm -f *.o

run: debug
	./client

gdb: debug
	gdb ./client --eval-command="run"

data: $(HOME)/.minecraft/bin/minecraft.jar
	unzip $(HOME)/.minecraft/bin/minecraft.jar -d data

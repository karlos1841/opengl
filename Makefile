CC=g++
CFLAGS=-Wall -Wextra -Werror -std=c++11
LDFLAGS=-lSDL2 -lGLEW
CFILES=main.cpp
EXEC=opengl

all: $(CFILES)
	$(CC) $(CFLAGS) -o $(EXEC) $(CFILES) $(LDFLAGS)

clean:
	rm -f $(EXEC)

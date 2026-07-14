CC      = gcc
CFLAGS  = -I include -Wall -Wextra -g -O2 $(shell sdl2-config --cflags)
LDFLAGS = $(shell sdl2-config --libs) -lSDL2_image -lSDL2_mixer -lm
SRC     = src/main.c src/game.c src/render.c src/physics.c src/input.c src/audio.c
OBJ     = $(SRC:.c=.o)
TARGET  = flappy_bird

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET) $(LDFLAGS)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run

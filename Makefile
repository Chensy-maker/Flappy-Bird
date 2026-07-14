CC = gcc
CFLAGS = -I include -Wall -g
LDFLAGS = -lSDL2 -lSDL2_image -lm
SRC = src/main.c src/game.c src/render.c src/physics.c src/input.c
OBJ = $(SRC:.c=.o)
TARGET = flappy_bird
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

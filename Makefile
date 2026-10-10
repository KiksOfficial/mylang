CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -Iinclude

TARGET = mylang

SRC = main.c \
      src/lexer.c \

OBJ = $(SRC:.c=.o)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJ) $(TARGET)

run: $(TARGET)
	./$(TARGET)

.PHONY: clean run

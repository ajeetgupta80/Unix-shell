CC = gcc
CFLAGS = -Wall -Werror -Wextra -pedantic
SRC_DIR = src
OBJ_DIR = obj
BIN = edSh

SRC_FILES = $(wildcard $(SRC_DIR)/*.c)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC_FILES))

all: $(BIN)

$(BIN): $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $(BIN) $(OBJ_FILES)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c | $(OBJ_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

clean:
	rm -f $(OBJ_DIR)/*.o $(BIN)

.PHONY: all clean
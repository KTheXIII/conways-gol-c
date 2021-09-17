CC?=clang
SDL2_INCLUDES = $(shell pkg-config sdl2 --cflags)
SDL2_LIB      = $(shell pkg-config sdl2 --libs)
CFLAGS  = -std=c18 -Wextra -Wall -Wpedantic -Werror -Wshadow -Wstrict-aliasing -Wstrict-overflow
CFLAGS += -Isrc $(SDL2_INCLUDES)

.PHONY: all msg clean

BIN_DIR = bin
OBJ_DIR = obj
SRC_DIR = src

SRC_FILES = $(wildcard $(SRC_DIR)/**.c)                             # Find all the .c files in SRC_DIR
OBJ_FILES = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SRC_FILES))  # Replace files with .c to .o

all: target_dir $(BIN_DIR)/conway

$(BIN_DIR)/conway: $(OBJ_FILES)
	$(CC) $(CFLAGS) -o $@ $^ $(SDL2_LIB)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR)/* $(OBJ_DIR)/*

target_dir:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)


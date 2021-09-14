CC?=clang
SDL2_INCLUDES = $(shell pkg-config sdl2 --cflags)
SDL2_LIB      = $(shell pkg-config sdl2 --libs)
CFLAGS  = -std=c18 -Wextra -Wall -Wpedantic -Werror -Wshadow -Wstrict-aliasing -Wstrict-overflow
CFLAGS += -Isrc $(SDL2_INCLUDES)


.PHONY: all msg clean fullclean

BIN_DIR = bin
OBJ_DIR = obj

GENERATED := $(OBJ_DIR)/main.o
GENERATED += $(OBJ_DIR)/GOL_Image.o
GENERATED += $(OBJ_DIR)/GOL_Board.o

all: outdir $(BIN_DIR)/conway

outdir:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)

$(BIN_DIR)/conway: $(GENERATED)
	$(CC) $(CFLAGS) -o $@ $^ $(SDL2_LIB)

$(OBJ_DIR)/main.o: src/main.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/GOL_Image.o: src/GOL_Image.c
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ_DIR)/GOL_Board.o: src/GOL_Board.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

fullclean: clean


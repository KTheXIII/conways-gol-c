CC?=clang
SDL2FLAGS=$(shell pkg-config sdl2 --cflags --libs)
CFLAGS  = -std=c18 -Wextra -Wall -pedantic -Werror -Wshadow -Wstrict-aliasing -Wstrict-overflow
CFLAGS += -Isrc

.PHONY: all msg clean fullclean

BIN_DIR     = bin
OBJ_DIR     = obj

GENERATED := $(OBJ_DIR)/main.o

all: outdir $(BIN_DIR)/conway

outdir:
	@mkdir -p $(BIN_DIR)
	@mkdir -p $(OBJ_DIR)

$(BIN_DIR)/conway: $(GENERATED)
	$(CC) $(CFLAGS) -o $@ $^ $(SDL2FLAGS)

$(OBJ_DIR)/main.o: src/main.c
	$(CC) $(CFLAGS) -c $< -o $@
clean:
	rm -rf $(BIN_DIR) $(OBJ_DIR)

fullclean: clean


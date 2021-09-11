#include "GOL_Board.h"

#include <stdlib.h>

GOL_Board* GOL_ConstructBoard(int32_t width, int32_t height) {
    GOL_Board* board    = (GOL_Board*)malloc(sizeof(GOL_Board));
    board->width    = width;
    board->height   = height;
    board->cells    = (uint8_t*)malloc(width * height * sizeof(uint8_t));
    board->cells_cp = (uint8_t*)malloc(width * height * sizeof(uint8_t));
    return board;
}
void GOL_DestructBoard(GOL_Board* board) {
    free(board->cells);
    free(board->cells_cp);
    free(board);
}
void GOL_BoardResize(GOL_Board* board, int32_t width, int32_t height) {
    board->width    = width;
    board->height   = height;
    board->cells    = (uint8_t*)realloc(board->cells,    width * height * sizeof(uint8_t));
    board->cells_cp = (uint8_t*)realloc(board->cells_cp, width * height * sizeof(uint8_t));
}
void GOL_BoardSimulate(GOL_Board* board) {
    int32_t const width  = board->width;
    int32_t const height = board->height;
    int32_t neighbors = 0;
    uint8_t state     = 0;
    for (int32_t i = 0; i < width * height; i++)
        board->cells_cp[i] = board->cells[i];

    for (int32_t i = 0; i < height; i++) {
        for (int32_t j = 0; j < width; j++) {
            neighbors = GOL_BoardCountNeighbors(board, j, i);
            state     = GOL_BoardGetCellCPState(board, j, i);

            if (state == 0 && neighbors == 3)
                state = 1;
            else if (state == 1 && (neighbors < 2 || neighbors > 3))
                state = 0;
            else
                state = GOL_BoardGetCellCPState(board, j, i);

            GOL_BoardSetCellState(board, j, i, state);
        }
    }
}
void GOL_BoardSetCellState(GOL_Board* board, int32_t x, int32_t y, uint8_t state) {
    if (x > -1 && x < board->width && y > -1 && y < board->height)
        board->cells[y * board->width + x] = state;
}
void GOL_BoardSetBothCellState(GOL_Board* board, int32_t x, int32_t y, uint8_t state) {
    if (x > -1 && x < board->width && y > -1 && y < board->height) {
        board->cells[y * board->width + x]    = state;
        board->cells_cp[y * board->width + x] = state;
    }
}
uint8_t GOL_BoardGetCellCPState(GOL_Board* board, int32_t x, int32_t y) {
    if (x > -1 && x < board->width && y > -1 && y < board->height)
        return board->cells_cp[y * board->width + x];
    else
        return 0;
}
uint8_t GOL_BoardGetCellState(GOL_Board* board, int32_t x, int32_t y) {
    if (x > -1 && x < board->width && y > -1 && y < board->height)
        return board->cells[y * board->width + x];
    else
        return 0;
}
int32_t GOL_BoardCountNeighbors(GOL_Board* board, int32_t x, int32_t y) {
    int32_t count = 0;
    int32_t nx, ny;
    uint8_t state = 0;
    for (int32_t i = -1; i < 2; i++) {
        ny = y + i;
        for (int32_t j = -1; j < 2; j++) {
            nx = x + j;

            state = GOL_BoardGetCellCPState(board, nx, ny);
            if (state && nx != x && ny != y)
                count++;
        }
    }
    return count;
}


#ifndef GOL_BOARD_H_
#define GOL_BOARD_H_

#include <stdint.h>

typedef struct {
    int32_t width;
    int32_t height;

    uint8_t* cells;
    uint8_t* cells_cp;
} GOL_Board;

/**
 * \brief Construct Game of Life Board.
 *
 * \param width  Cell width
 * \param height Cell height
 *
 * \return GOL_Board
 */
GOL_Board* GOL_ConstructBoard(int32_t width, int32_t height);
void GOL_DestructBoard(GOL_Board* board);
void GOL_BoardResize(GOL_Board* board, int32_t width, int32_t height);

void GOL_BoardSimulate(GOL_Board* board);

void GOL_BoardSetCellState(GOL_Board* board, int32_t x, int32_t y, uint8_t state);
void GOL_BoardSetBothCellState(GOL_Board* board, int32_t x, int32_t y, uint8_t state);

uint8_t GOL_BoardGetCellState(GOL_Board* board, int32_t x, int32_t y);
uint8_t GOL_BoardGetCellCPState(GOL_Board* board, int32_t x, int32_t y);
int32_t GOL_BoardCountNeighbors(GOL_Board* board, int32_t x, int32_t y);

#endif


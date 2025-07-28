#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H

#include <M5Unified.h>

namespace games_sudoku {
    void drawGameScreen();
    void handleTouch(int touchedRow, int x, int y);
    void initGame();
    void resetGame();
}

#endif
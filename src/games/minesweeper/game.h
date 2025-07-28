#ifndef MINESWEEPER_GAME_H
#define MINESWEEPER_GAME_H

#include <M5Unified.h>

namespace games_minesweeper {
    void drawGameScreen();
    void handleTouch(int touchType, int x, int y);
    void initGame();
    void resetGame();
}

#endif
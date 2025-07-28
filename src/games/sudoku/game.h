#ifndef SUDOKU_GAME_H
#define SUDOKU_GAME_H

#include <M5Unified.h>

namespace games_sudoku {
    void drawGameScreen();
    void handleTouch(int touchType, int x, int y);
    void initGame();
    void resetGame();
    void handleKeyboardInput(const String& key);
    void hideKeyboard();
}

#endif
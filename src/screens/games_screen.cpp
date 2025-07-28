#include "games_screen.h"
#include "../ui.h"
#include "../games/minesweeper/game.h"
#include "../games/sudoku/game.h"
#include "../games/test/game.h"

const String installedGames[] = {"Minesweeper", "Sudoku", "Test"};
const int installedGamesCount = 3;

void drawGamesScreen() {
    ::updateHeader();

    RowPosition contentStart = getRowPosition(2);
    RowPosition footerStart = getRowPosition(15);
    M5.Display.fillRect(contentStart.x, contentStart.y, contentStart.width, footerStart.y - contentStart.y, TFT_WHITE);

    if (installedGamesCount == 0) {
        ::bufferRow("No games found.", 2, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
    } else {
        ::bufferRow("Games:", 2, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
        for (int i = 0; i < installedGamesCount; ++i) {
            ::bufferRow(installedGames[i], i + 3, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true);
        }
    }
}

void handleGamesScreenTouch(int touchedRow, int x, int y) {
    if (touchedRow - 3 >= 0 && touchedRow - 3 < installedGamesCount) {
        String selectedGame = installedGames[touchedRow - 3];
        
        if (selectedGame == "Minesweeper") {
            games_minesweeper::initGame();
            setCurrentScreen(MINESWEEPER_GAME_SCREEN);
        } else if (selectedGame == "Sudoku") {
            games_sudoku::initGame();
            setCurrentScreen(SUDOKU_GAME_SCREEN);
        } else if (selectedGame == "Test") {
            games_test::initGame();
            setCurrentScreen(TEST_GAME_SCREEN);
        }
    }
}
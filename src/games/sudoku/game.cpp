#include "game.h"
#include "../../ui.h"
#include <random>
#include <algorithm>
#include <vector>
#include <cstring>

namespace games_sudoku {
    static int board[9][9];
    static int solution[9][9];
    static bool readonly[9][9];
    static bool gameWon = false;
    static bool gameStarted = false;
    static int selectedRow = -1;
    static int selectedCol = -1;
    static const int CELL_SIZE = 40;
    static const int GRID_START_X = 50;
    static const int GRID_START_Y = 50;
    
    bool isValid(int grid[9][9], int row, int col, int num) {
        for (int x = 0; x < 9; x++) {
            if (grid[row][x] == num || grid[x][col] == num) {
                return false;
            }
        }
        
        int startRow = row - row % 3;
        int startCol = col - col % 3;
        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < 3; j++) {
                if (grid[i + startRow][j + startCol] == num) {
                    return false;
                }
            }
        }
        return true;
    }
    
    bool solveSudoku(int grid[9][9]) {
        for (int row = 0; row < 9; row++) {
            for (int col = 0; col < 9; col++) {
                if (grid[row][col] == 0) {
                    std::vector<int> numbers = {1, 2, 3, 4, 5, 6, 7, 8, 9};
                    std::random_device rd;
                    std::mt19937 rng(rd());
                    std::shuffle(numbers.begin(), numbers.end(), rng);
                    
                    for (int num : numbers) {
                        if (isValid(grid, row, col, num)) {
                            grid[row][col] = num;
                            if (solveSudoku(grid)) {
                                return true;
                            }
                            grid[row][col] = 0;
                        }
                    }
                    return false;
                }
            }
        }
        return true;
    }
    
    void generatePuzzle() {
        memset(solution, 0, sizeof(solution));
        memset(readonly, false, sizeof(readonly));
        solveSudoku(solution);
        
        memcpy(board, solution, sizeof(board));
        
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                readonly[i][j] = true;
            }
        }
        
        std::vector<std::pair<int, int>> cells;
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                cells.push_back({i, j});
            }
        }
        
        std::random_device rd;
        std::mt19937 rng(rd());
        std::shuffle(cells.begin(), cells.end(), rng);
        
        int cellsToRemove = 40 + (rng() % 21);
        for (int i = 0; i < cellsToRemove && i < cells.size(); i++) {
            int row = cells[i].first;
            int col = cells[i].second;
            board[row][col] = 0;
            readonly[row][col] = false;
        }
        
        gameWon = false;
        gameStarted = true;
        selectedRow = -1;
        selectedCol = -1;
    }
    
    bool checkWin() {
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                if (board[i][j] == 0 || board[i][j] != solution[i][j]) {
                    return false;
                }
            }
        }
        return true;
    }
    
    void setCell(int row, int col, int value) {
        if (row >= 0 && row < 9 && col >= 0 && col < 9 && !readonly[row][col]) {
            board[row][col] = value;
            if (checkWin()) {
                gameWon = true;
            }
        }
    }
    
    void drawGameScreen() {
        M5.Display.clear();
        M5.Display.setTextColor(BLACK);
        M5.Display.setTextSize(2);
        
        // Game status
        if (gameWon) {
            M5.Display.drawString("YOU WIN!", 10, 10);
        } else if (gameStarted) {
            M5.Display.drawString("Playing...", 10, 10);
        }
        
        // Draw grid
        for (int i = 0; i < 9; i++) {
            for (int j = 0; j < 9; j++) {
                int x = GRID_START_X + j * CELL_SIZE;
                int y = GRID_START_Y + i * CELL_SIZE;
                
                // Highlight selected cell
                if (i == selectedRow && j == selectedCol) {
                    M5.Display.fillRect(x, y, CELL_SIZE, CELL_SIZE, YELLOW);
                }
                
                // Draw cell border (thicker for 3x3 blocks)
                int borderWidth = ((i % 3 == 0) || (j % 3 == 0)) ? 2 : 1;
                M5.Display.drawRect(x, y, CELL_SIZE, CELL_SIZE, BLACK);
                if (borderWidth == 2) {
                    M5.Display.drawRect(x + 1, y + 1, CELL_SIZE - 2, CELL_SIZE - 2, BLACK);
                }
                
                // Draw number
                if (board[i][j] != 0) {
                    M5.Display.setTextSize(1);
                    if (readonly[i][j]) {
                        M5.Display.setTextColor(BLACK);
                    } else {
                        M5.Display.setTextColor(BLUE);
                    }
                    M5.Display.drawString(String(board[i][j]), x + CELL_SIZE/2 - 5, y + CELL_SIZE/2 - 8);
                    M5.Display.setTextColor(BLACK);
                    M5.Display.setTextSize(2);
                }
            }
        }
        
        // Draw number buttons
        M5.Display.setTextSize(1);
        for (int i = 1; i <= 9; i++) {
            int x = 450 + ((i - 1) % 3) * 30;
            int y = 100 + ((i - 1) / 3) * 30;
            M5.Display.drawRect(x, y, 25, 25, BLACK);
            M5.Display.drawString(String(i), x + 8, y + 8);
        }
        
        // Clear button
        M5.Display.drawRect(450, 190, 25, 25, BLACK);
        M5.Display.drawString("C", 458, 198);
        
        // Control buttons
        M5.Display.setTextSize(2);
        M5.Display.drawString("New Game", 450, 250);
        M5.Display.drawString("Back", 450, 280);
    }
    
    void handleTouch(int touchedRow, int x, int y) {
        // Check control buttons
        if (x >= 450) {
            if (y >= 250 && y <= 280) {
                // New Game button
                resetGame();
                return;
            } else if (y >= 280 && y <= 310) {
                // Back button
                setCurrentScreen(GAMES_SCREEN);
                return;
            }
            
            // Check number buttons
            if (x >= 450 && x <= 540 && y >= 100 && y <= 190) {
                int buttonX = (x - 450) / 30;
                int buttonY = (y - 100) / 30;
                if (buttonX < 3 && buttonY < 3) {
                    int number = buttonY * 3 + buttonX + 1;
                    if (selectedRow >= 0 && selectedCol >= 0) {
                        setCell(selectedRow, selectedCol, number);
                    }
                }
                return;
            }
            
            // Check clear button
            if (x >= 450 && x <= 475 && y >= 190 && y <= 215) {
                if (selectedRow >= 0 && selectedCol >= 0) {
                    setCell(selectedRow, selectedCol, 0);
                }
                return;
            }
        }
        
        // Check if touch is in grid
        if (x >= GRID_START_X && x < GRID_START_X + 9 * CELL_SIZE &&
            y >= GRID_START_Y && y < GRID_START_Y + 9 * CELL_SIZE) {
            
            int col = (x - GRID_START_X) / CELL_SIZE;
            int row = (y - GRID_START_Y) / CELL_SIZE;
            
            if (!gameStarted) {
                generatePuzzle();
            }
            
            selectedRow = row;
            selectedCol = col;
        }
    }
    
    void initGame() {
        gameWon = false;
        gameStarted = false;
        selectedRow = -1;
        selectedCol = -1;
        memset(board, 0, sizeof(board));
        memset(solution, 0, sizeof(solution));
        memset(readonly, false, sizeof(readonly));
    }
    
    void resetGame() {
        initGame();
        generatePuzzle();
    }
}
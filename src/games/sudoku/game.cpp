#include "game.h"
#include "../../ui.h"
#include "../../keyboards/numbers_keyboard.h"
#include <random>
#include <algorithm>
#include <vector>
#include <cstring>

namespace games_sudoku {
    static const int SCREEN_WIDTH = 540;
    static const int SCREEN_HEIGHT = 960;
    static const int GRID_SIZE = 6;
    static const int BLOCK_SIZE = 3;
    static const int CELL_SIZE = 80;
    static const int BLOCK_SPACING = 20;
    static const int TOTAL_GRID_WIDTH = (GRID_SIZE * CELL_SIZE) + BLOCK_SPACING;
    static const int TOTAL_GRID_HEIGHT = (GRID_SIZE * CELL_SIZE) + BLOCK_SPACING;
    static const int GRID_START_X = (SCREEN_WIDTH - TOTAL_GRID_WIDTH) / 2;
    static const int GRID_START_Y = (SCREEN_HEIGHT - TOTAL_GRID_HEIGHT) / 2;
    
    static int board[GRID_SIZE][GRID_SIZE];
    static int solution[GRID_SIZE][GRID_SIZE];
    static bool readonly[GRID_SIZE][GRID_SIZE];
    static bool gameWon = false;
    static bool gameStarted = false;
    static int selectedRow = -1;
    static int selectedCol = -1;
    static bool keyboardVisible = false;
    static bool errorCells[GRID_SIZE][GRID_SIZE];
    static unsigned long errorDisplayTime = 0;
    static bool showingErrors = false;
    static bool showingErrorMessage = false;
    static unsigned long errorMessageStartTime = 0;
    static bool showingHint = false;
    
    bool isValid(int grid[GRID_SIZE][GRID_SIZE], int row, int col, int num) {
        for (int x = 0; x < GRID_SIZE; x++) {
            if (grid[row][x] == num || grid[x][col] == num) {
                return false;
            }
        }
        
        int blockRow = row / BLOCK_SIZE;
        int blockCol = col / BLOCK_SIZE;
        int startRow = blockRow * BLOCK_SIZE;
        int startCol = blockCol * BLOCK_SIZE;
        
        for (int i = 0; i < BLOCK_SIZE; i++) {
            for (int j = 0; j < BLOCK_SIZE; j++) {
                if (grid[startRow + i][startCol + j] == num) {
                    return false;
                }
            }
        }
        return true;
    }
    
    bool solveSudoku(int grid[GRID_SIZE][GRID_SIZE]) {
        for (int row = 0; row < GRID_SIZE; row++) {
            for (int col = 0; col < GRID_SIZE; col++) {
                if (grid[row][col] == 0) {
                    std::vector<int> numbers = {1, 2, 3, 4, 5, 6};
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
        int predefinedBoards[4][6][6] = {
            {
                {1, 2, 3, 4, 5, 6},
                {4, 5, 6, 1, 2, 3},
                {2, 3, 1, 6, 4, 5},
                {5, 6, 4, 3, 1, 2},
                {3, 1, 2, 5, 6, 4},
                {6, 4, 5, 2, 3, 1}
            },
            {
                {6, 4, 5, 2, 3, 1},
                {3, 1, 2, 5, 6, 4},
                {5, 6, 4, 3, 1, 2},
                {2, 3, 1, 6, 4, 5},
                {4, 5, 6, 1, 2, 3},
                {1, 2, 3, 4, 5, 6}
            },
            {
                {2, 3, 1, 6, 4, 5},
                {5, 6, 4, 3, 1, 2},
                {1, 2, 3, 4, 5, 6},
                {4, 5, 6, 1, 2, 3},
                {6, 4, 5, 2, 3, 1},
                {3, 1, 2, 5, 6, 4}
            },
            {
                {4, 5, 6, 1, 2, 3},
                {1, 2, 3, 4, 5, 6},
                {3, 1, 2, 5, 6, 4},
                {6, 4, 5, 2, 3, 1},
                {2, 3, 1, 6, 4, 5},
                {5, 6, 4, 3, 1, 2}
            }
        };
        
        static unsigned long lastSeed = 0;
        unsigned long currentTime = millis();
        if (currentTime != lastSeed) {
            lastSeed = currentTime;
            srand(currentTime);
        }
        
        int boardIndex = rand() % 4;
        
        memset(readonly, false, sizeof(readonly));
        
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                board[i][j] = predefinedBoards[boardIndex][i][j];
                solution[i][j] = predefinedBoards[boardIndex][i][j];
                readonly[i][j] = true;
            }
        }
        
        std::vector<std::pair<int, int>> cells;
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                cells.push_back({i, j});
            }
        }
        
        std::random_device rd;
        std::mt19937 rng(currentTime);
        std::shuffle(cells.begin(), cells.end(), rng);
        
        int cellsToRemove = 28 + (rand() % 4);
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
        keyboardVisible = false;
    }
    
    bool isBoardFull() {
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                if (board[i][j] == 0) {
                    return false;
                }
            }
        }
        return true;
    }

    bool checkWin() {
        bool boardFull = isBoardFull();
        
        if (!boardFull) {
            return false;
        }
        
        for (int i = 0; i < GRID_SIZE; i++) {
            for (int j = 0; j < GRID_SIZE; j++) {
                if (board[i][j] != solution[i][j]) {
                    showingErrorMessage = true;
                    errorMessageStartTime = millis();
                    return false;
                }
            }
        }
        
        return true;
    }
    
    void checkLines() {
        memset(errorCells, false, sizeof(errorCells));
        
        for (int row = 0; row < GRID_SIZE; row++) {
            bool hasError = false;
            for (int col1 = 0; col1 < GRID_SIZE; col1++) {
                if (board[row][col1] == 0) continue;
                for (int col2 = col1 + 1; col2 < GRID_SIZE; col2++) {
                    if (board[row][col1] == board[row][col2]) {
                        hasError = true;
                        break;
                    }
                }
                if (hasError) break;
            }
            if (hasError) {
                for (int col = 0; col < GRID_SIZE; col++) {
                    if (board[row][col] != 0) {
                        errorCells[row][col] = true;
                    }
                }
            }
        }
        
        for (int col = 0; col < GRID_SIZE; col++) {
            bool hasError = false;
            for (int row1 = 0; row1 < GRID_SIZE; row1++) {
                if (board[row1][col] == 0) continue;
                for (int row2 = row1 + 1; row2 < GRID_SIZE; row2++) {
                    if (board[row1][col] == board[row2][col]) {
                        hasError = true;
                        break;
                    }
                }
                if (hasError) break;
            }
            if (hasError) {
                for (int row = 0; row < GRID_SIZE; row++) {
                    if (board[row][col] != 0) {
                        errorCells[row][col] = true;
                    }
                }
            }
        }
        
        for (int blockRow = 0; blockRow < 2; blockRow++) {
            for (int blockCol = 0; blockCol < 2; blockCol++) {
                bool hasError = false;
                for (int i1 = 0; i1 < BLOCK_SIZE && !hasError; i1++) {
                    for (int j1 = 0; j1 < BLOCK_SIZE && !hasError; j1++) {
                        int row1 = blockRow * BLOCK_SIZE + i1;
                        int col1 = blockCol * BLOCK_SIZE + j1;
                        if (board[row1][col1] == 0) continue;
                        
                        for (int i2 = 0; i2 < BLOCK_SIZE && !hasError; i2++) {
                            for (int j2 = 0; j2 < BLOCK_SIZE && !hasError; j2++) {
                                if (i1 == i2 && j1 == j2) continue;
                                int row2 = blockRow * BLOCK_SIZE + i2;
                                int col2 = blockCol * BLOCK_SIZE + j2;
                                if (board[row1][col1] == board[row2][col2]) {
                                    hasError = true;
                                }
                            }
                        }
                    }
                }
                
                if (hasError) {
                    for (int i = 0; i < BLOCK_SIZE; i++) {
                        for (int j = 0; j < BLOCK_SIZE; j++) {
                            int row = blockRow * BLOCK_SIZE + i;
                            int col = blockCol * BLOCK_SIZE + j;
                            if (board[row][col] != 0) {
                                errorCells[row][col] = true;
                            }
                        }
                    }
                }
            }
        }
        
        showingErrors = true;
        errorDisplayTime = millis();
    }
    
    void setCell(int row, int col, int value) {
        if (row >= 0 && row < GRID_SIZE && col >= 0 && col < GRID_SIZE && !readonly[row][col]) {
            board[row][col] = value;
            if (checkWin()) {
                gameWon = true;
            }
        }
    }
    
    void drawGameScreen() {
        if (showingErrors && millis() - errorDisplayTime > 2000) {
            showingErrors = false;
        }
        
        if (showingErrorMessage && millis() - errorMessageStartTime > 3000) {
            showingErrorMessage = false;
        }
        

        
        if (!gameWon && checkWin()) {
            gameWon = true;
        }
        
        M5.Display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
        M5.Display.setTextColor(BLACK);
        M5.Display.setTextSize(3);
        
        int restartWidth = M5.Display.textWidth("RESTART");
        int restartX = (SCREEN_WIDTH - restartWidth) / 2;
        M5.Display.drawString("RESTART", restartX, 30);
        M5.Display.drawLine(restartX, 60, restartX + restartWidth, 60, BLACK);
        
        int hintWidth = M5.Display.textWidth("HINT");
        int hintX = (SCREEN_WIDTH - hintWidth) / 2;
        int hintY = SCREEN_HEIGHT - 90;
        M5.Display.drawString("HINT", hintX, hintY);
        M5.Display.drawLine(hintX, hintY + 30, hintX + hintWidth, hintY + 30, BLACK);
        
        if (gameWon) {
            M5.Display.setTextColor(BLACK);
            M5.Display.drawString("YOU WIN!", 200, 100);
        } else if (showingErrorMessage) {
            M5.Display.setTextColor(BLACK);
            M5.Display.drawString("ERROR!", 220, 100);
        }
        
        for (int blockRow = 0; blockRow < 2; blockRow++) {
            for (int blockCol = 0; blockCol < 2; blockCol++) {
                int blockStartX = GRID_START_X + blockCol * (BLOCK_SIZE * CELL_SIZE + BLOCK_SPACING);
                int blockStartY = GRID_START_Y + blockRow * (BLOCK_SIZE * CELL_SIZE + BLOCK_SPACING);
                
                M5.Display.drawRect(blockStartX - 2, blockStartY - 2, 
                                  BLOCK_SIZE * CELL_SIZE + 4, BLOCK_SIZE * CELL_SIZE + 4, BLACK);
                M5.Display.drawRect(blockStartX - 3, blockStartY - 3, 
                                  BLOCK_SIZE * CELL_SIZE + 6, BLOCK_SIZE * CELL_SIZE + 6, BLACK);
                
                for (int i = 0; i < BLOCK_SIZE; i++) {
                    for (int j = 0; j < BLOCK_SIZE; j++) {
                        int gridRow = blockRow * BLOCK_SIZE + i;
                        int gridCol = blockCol * BLOCK_SIZE + j;
                        
                        int x = blockStartX + j * CELL_SIZE;
                        int y = blockStartY + i * CELL_SIZE;
                        
                        uint16_t cellColor = WHITE;
                         if (showingErrors && errorCells[gridRow][gridCol]) {
                             cellColor = RED;
                         } else if (gridRow == selectedRow && gridCol == selectedCol && !readonly[gridRow][gridCol]) {
                             cellColor = YELLOW;
                         } else if (readonly[gridRow][gridCol]) {
                             cellColor = LIGHTGREY;
                         }
                         
                         M5.Display.fillRect(x + 1, y + 1, CELL_SIZE - 2, CELL_SIZE - 2, cellColor);
                        
                        M5.Display.drawRect(x, y, CELL_SIZE, CELL_SIZE, BLACK);
                        
                        if (board[gridRow][gridCol] != 0) {
                            M5.Display.setTextSize(4);
                            M5.Display.setTextColor(BLACK);
                            String numStr = String(board[gridRow][gridCol]);
                            int textWidth = M5.Display.textWidth(numStr);
                            int textHeight = M5.Display.fontHeight();
                            int textX = x + (CELL_SIZE - textWidth) / 2;
                            int textY = y + (CELL_SIZE - textHeight) / 2;
                            M5.Display.drawString(numStr, textX, textY);
                        }
                    }
                }
            }
        }
        
        if (keyboardVisible) {
            keyboards::drawNumbersKeyboard();
        }
        
        if (showingHint) {
            int hintWidth = 480;
            int hintHeight = 360;
            int hintX = (SCREEN_WIDTH - hintWidth) / 2;
            int hintY = (SCREEN_HEIGHT - hintHeight) / 2;
            
            M5.Display.fillRect(hintX, hintY, hintWidth, hintHeight, LIGHTGREY);
            M5.Display.drawRect(hintX, hintY, hintWidth, hintHeight, BLACK);
            M5.Display.setTextColor(BLACK);
            M5.Display.setTextSize(3);
            
            int titleWidth = M5.Display.textWidth("Solution:");
            int titleX = (SCREEN_WIDTH - titleWidth) / 2;
            M5.Display.drawString("Solution:", titleX, hintY + 30);
            
            M5.Display.setTextSize(2);
            for (int i = 0; i < GRID_SIZE; i++) {
                String row = "";
                for (int j = 0; j < GRID_SIZE; j++) {
                    row += String(solution[i][j]);
                    if (j < GRID_SIZE - 1) row += "  ";
                }
                int rowWidth = M5.Display.textWidth(row);
                int rowX = (SCREEN_WIDTH - rowWidth) / 2;
                M5.Display.drawString(row, rowX, hintY + 80 + i * 40);
            }
            M5.Display.setTextSize(3);
        }
    }
    
    void handleTouch(int touchType, int x, int y) {
        M5.Display.setTextSize(3);
        int textWidth = M5.Display.textWidth("RESTART");
        int restartX = (SCREEN_WIDTH - textWidth) / 2;
        if (x >= restartX && x <= restartX + textWidth && y >= 15 && y <= 65) {
            resetGame();
            return;
        }
        

        
        int hintWidth = M5.Display.textWidth("HINT");
        int hintX = (SCREEN_WIDTH - hintWidth) / 2;
        int hintY = SCREEN_HEIGHT - 90;
        
        if (x >= hintX && x <= hintX + hintWidth && y >= hintY && y <= hintY + 60) {
            showingHint = !showingHint;
            return;
        }
        
        if (gameWon) {
            return;
        }
        
        if (keyboardVisible) {
            String key = keyboards::getNumberKeyFromTouch(x, y);
            if (key != "") {
                handleKeyboardInput(key);
                return;
            }
        }
        
        for (int blockRow = 0; blockRow < 2; blockRow++) {
            for (int blockCol = 0; blockCol < 2; blockCol++) {
                int blockStartX = GRID_START_X + blockCol * (BLOCK_SIZE * CELL_SIZE + BLOCK_SPACING);
                int blockStartY = GRID_START_Y + blockRow * (BLOCK_SIZE * CELL_SIZE + BLOCK_SPACING);
                
                if (x >= blockStartX && x < blockStartX + BLOCK_SIZE * CELL_SIZE &&
                    y >= blockStartY && y < blockStartY + BLOCK_SIZE * CELL_SIZE) {
                    
                    int localCol = (x - blockStartX) / CELL_SIZE;
                    int localRow = (y - blockStartY) / CELL_SIZE;
                    
                    int gridRow = blockRow * BLOCK_SIZE + localRow;
                    int gridCol = blockCol * BLOCK_SIZE + localCol;
                    
                    if (gridRow < 0 || gridRow >= GRID_SIZE || gridCol < 0 || gridCol >= GRID_SIZE) return;
                    
                    if (!readonly[gridRow][gridCol]) {
                        selectedRow = gridRow;
                        selectedCol = gridCol;
                        keyboardVisible = true;
                    }
                    return;
                }
            }
        }
        
        keyboardVisible = false;
        selectedRow = -1;
        selectedCol = -1;
    }
    
    void handleKeyboardInput(const String& key) {
        if (selectedRow >= 0 && selectedCol >= 0 && !readonly[selectedRow][selectedCol]) {
            if (key == "<") {
                setCell(selectedRow, selectedCol, 0);
            } else {
                int num = key.toInt();
                if (num >= 1 && num <= 6) {
                    setCell(selectedRow, selectedCol, num);
                }
            }
            keyboardVisible = false;
            selectedRow = -1;
            selectedCol = -1;
        }
    }
    
    void hideKeyboard() {
        keyboardVisible = false;
        selectedRow = -1;
        selectedCol = -1;
    }
    
    void initGame() {
        gameWon = false;
        gameStarted = false;
        selectedRow = -1;
        selectedCol = -1;
        keyboardVisible = false;
        memset(board, 0, sizeof(board));
        memset(solution, 0, sizeof(solution));
        memset(readonly, false, sizeof(readonly));
        generatePuzzle();
    }
    
    void resetGame() {
        initGame();
        generatePuzzle();
    }
}
#include "game.h"
#include "../../ui.h"
#include <random>
#include <algorithm>
#include <vector>
#include <cstring>

namespace games_minesweeper {
    static const int SCREEN_WIDTH = 540;
    static const int SCREEN_HEIGHT = 960;
    static const int RESTART_BUTTON_HEIGHT = 60;
    
    static const int GRID_WIDTH = 10;
    static const int GRID_HEIGHT = 15;
    static const int MINE_COUNT = 25;
    static const int CELL_SIZE = 54;
    static const int GRID_START_X = 0;
    static const int GRID_START_Y = SCREEN_HEIGHT - (GRID_HEIGHT * CELL_SIZE);
    
    enum CellState {
        HIDDEN = 0,
        REVEALED = 1
    };
    
    static bool mines[GRID_HEIGHT][GRID_WIDTH];
    static int neighborCounts[GRID_HEIGHT][GRID_WIDTH];
    static CellState cellStates[GRID_HEIGHT][GRID_WIDTH];
    static bool gameWon = false;
    static bool gameLost = false;
    static bool gameStarted = false;
    static int revealedCount = 0;
    static int flagCount = 0;
    
    void generateMines(int firstClickRow, int firstClickCol) {
        memset(mines, false, sizeof(mines));
        memset(neighborCounts, 0, sizeof(neighborCounts));
        
        std::vector<std::pair<int, int>> positions;
        for (int i = 0; i < GRID_HEIGHT; i++) {
            for (int j = 0; j < GRID_WIDTH; j++) {
                if (i != firstClickRow || j != firstClickCol) {
                    positions.push_back({i, j});
                }
            }
        }
        
        std::random_device rd;
        std::mt19937 rng(rd());
        std::shuffle(positions.begin(), positions.end(), rng);
        
        for (int i = 0; i < MINE_COUNT && i < positions.size(); i++) {
            int row = positions[i].first;
            int col = positions[i].second;
            mines[row][col] = true;
        }
        
        for (int i = 0; i < GRID_HEIGHT; i++) {
            for (int j = 0; j < GRID_WIDTH; j++) {
                if (mines[i][j]) {
                    neighborCounts[i][j] = -1;
                    continue;
                }
                
                int count = 0;
                for (int di = -1; di <= 1; di++) {
                    for (int dj = -1; dj <= 1; dj++) {
                        int ni = i + di;
                        int nj = j + dj;
                        if (ni >= 0 && ni < GRID_HEIGHT && nj >= 0 && nj < GRID_WIDTH) {
                            if (mines[ni][nj]) count++;
                        }
                    }
                }
                neighborCounts[i][j] = count;
            }
        }
        gameStarted = true;
    }
    

    
    void revealCell(int row, int col) {
        if (row < 0 || row >= GRID_HEIGHT || col < 0 || col >= GRID_WIDTH) return;
        if (cellStates[row][col] != HIDDEN) return;
        if (gameLost || gameWon) return;
        
        cellStates[row][col] = REVEALED;
        revealedCount++;
        
        if (mines[row][col]) {
            gameLost = true;
            return;
        }
        
        if (neighborCounts[row][col] == 0) {
            for (int di = -1; di <= 1; di++) {
                for (int dj = -1; dj <= 1; dj++) {
                    revealCell(row + di, col + dj);
                }
            }
        }
        
        if (revealedCount == GRID_WIDTH * GRID_HEIGHT - MINE_COUNT) {
            gameWon = true;
        }
    }
    

    

    
    void drawGameScreen() {
        M5.Display.fillRect(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, WHITE);
        M5.Display.setTextColor(BLACK);
        M5.Display.setTextSize(2);
        
        M5.Display.setTextSize(3);
        int textWidth = M5.Display.textWidth("RESTART");
        int restartX = (SCREEN_WIDTH - textWidth) / 2;
        M5.Display.drawString("RESTART", restartX, 35);
        M5.Display.drawLine(restartX, 65, restartX + textWidth, 65, BLACK);
        
        for (int i = 0; i < GRID_HEIGHT; i++) {
            for (int j = 0; j < GRID_WIDTH; j++) {
                int x = GRID_START_X + j * CELL_SIZE;
                int y = GRID_START_Y + i * CELL_SIZE;
                
                M5.Display.drawRect(x, y, CELL_SIZE, CELL_SIZE, BLACK);
                
                if (cellStates[i][j] == REVEALED || (gameLost && mines[i][j])) {
                    if (mines[i][j]) {
                        if (gameLost) {
                            M5.Display.fillRect(x + 1, y + 1, CELL_SIZE - 2, CELL_SIZE - 2, RED);
                        }
                        M5.Display.fillCircle(x + CELL_SIZE/2, y + CELL_SIZE/2, 8, BLACK);
                    } else {
                        M5.Display.fillRect(x + 1, y + 1, CELL_SIZE - 2, CELL_SIZE - 2, LIGHTGREY);
                        if (neighborCounts[i][j] > 0) {
                            M5.Display.setTextSize(6);
                            M5.Display.setTextColor(BLACK);
                            String numStr = String(neighborCounts[i][j]);
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
        
        if (gameWon) {
            M5.Display.fillRect(50, 300, 440, 100, GREEN);
            M5.Display.setTextColor(BLACK);
            M5.Display.setTextSize(3);
            M5.Display.drawString("YOU WIN!", 180, 340);
        } else if (gameLost) {
            M5.Display.fillRect(50, 300, 440, 100, RED);
            M5.Display.setTextColor(WHITE);
            M5.Display.setTextSize(3);
            M5.Display.drawString("GAME OVER", 160, 340);
        }
    }
    
    void handleTouch(int touchType, int x, int y) {
        M5.Display.setTextSize(3);
        int textWidth = M5.Display.textWidth("RESTART");
        int restartX = (SCREEN_WIDTH - textWidth) / 2;
        if (x >= restartX && x <= restartX + textWidth && y >= 20 && y <= 70) {
            resetGame();
            return;
        }
        
        if (x >= GRID_START_X && x < GRID_START_X + GRID_WIDTH * CELL_SIZE &&
            y >= GRID_START_Y && y < GRID_START_Y + GRID_HEIGHT * CELL_SIZE) {
            
            int col = (x - GRID_START_X) / CELL_SIZE;
            int row = (y - GRID_START_Y) / CELL_SIZE;
            
            if (row < 0 || row >= GRID_HEIGHT || col < 0 || col >= GRID_WIDTH) return;
            
            if (!gameStarted) {
                generateMines(row, col);
                revealCell(row, col);
            } else {
                revealCell(row, col);
            }
        }
    }
    
    void initGame() {
        gameWon = false;
        gameLost = false;
        gameStarted = false;
        revealedCount = 0;
        flagCount = 0;
        memset(mines, false, sizeof(mines));
        memset(neighborCounts, 0, sizeof(neighborCounts));
        memset(cellStates, HIDDEN, sizeof(cellStates));
    }
    
    void resetGame() {
        initGame();
    }
}
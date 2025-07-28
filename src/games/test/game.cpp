#include "game.h"
#include "../../ui.h"

namespace games_test {
    void drawGameScreen() {
        M5.Display.clear();
        M5.Display.setTextColor(BLACK);
        M5.Display.setTextSize(4);
        
        // Draw "test" in the center of the screen
        M5.Display.drawString("test", 200, 200);
    }
    
    void handleTouch(int touchedRow, int x, int y) {
        // No touch handling needed for test game
    }
    
    void initGame() {
        // Nothing to initialize for test game
    }
    
    void resetGame() {
        // Nothing to reset for test game
    }
}
#include "game.h"
#include "../../ui.h"

namespace games_test {
    void drawGameScreen() {
        M5.Display.clear();
        M5.Display.setTextColor(BLACK);
        M5.Display.setTextSize(4);
        
        int screenWidth = M5.Display.width();
        int screenHeight = M5.Display.height();
        
        String text = "Test";
        int textWidth = M5.Display.textWidth(text);
        int textHeight = M5.Display.fontHeight();
        
        int centerX = (screenWidth - textWidth) / 2;
        int centerY = (screenHeight - textHeight) / 2;
        
        M5.Display.drawString(text, centerX, centerY);
        
        drawDashedBorder();
    }
    
    void drawDashedBorder() {
        int screenWidth = M5.Display.width();
        int screenHeight = M5.Display.height();
        int dashLength = 10;
        int gapLength = 5;
        int thickness = 3;
        
        M5.Display.setColor(BLACK);
        
        for (int x = 0; x < screenWidth; x += dashLength + gapLength) {
            int lineEnd = min(x + dashLength, screenWidth);
            for (int t = 0; t < thickness; t++) {
                M5.Display.drawLine(x, t, lineEnd, t);
                M5.Display.drawLine(x, screenHeight - 1 - t, lineEnd, screenHeight - 1 - t);
            }
        }
        
        for (int y = 0; y < screenHeight; y += dashLength + gapLength) {
            int lineEnd = min(y + dashLength, screenHeight);
            for (int t = 0; t < thickness; t++) {
                M5.Display.drawLine(t, y, t, lineEnd);
                M5.Display.drawLine(screenWidth - 1 - t, y, screenWidth - 1 - t, lineEnd);
            }
        }
    }
    
    void handleTouch(int touchedRow, int x, int y) {
    }
    
    void initGame() {
    }
    
    void resetGame() {
    }
}
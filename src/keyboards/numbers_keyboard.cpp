#include "numbers_keyboard.h"
#include "../ui.h"

namespace keyboards {
    void drawNumbersKeyboard() {
        const int keyWidth = EPD_WIDTH / NUMBERS_KEYBOARD_COLS;
        const int keyHeight = 80;
        const int startY = EPD_HEIGHT - (NUMBERS_KEYBOARD_ROWS * keyHeight) - 60;

        M5.Display.fillRect(0, startY, EPD_WIDTH, NUMBERS_KEYBOARD_ROWS * keyHeight, TFT_WHITE);

        for (int row = 0; row < NUMBERS_KEYBOARD_ROWS; ++row) {
            for (int col = 0; col < NUMBERS_KEYBOARD_COLS; ++col) {
                int x = col * keyWidth;
                int y = startY + row * keyHeight;

                M5.Display.drawRect(x, y, keyWidth, keyHeight, TFT_BLACK);
                
                M5.Display.setCursor(x + (keyWidth - M5.Display.textWidth(NUMBERS_KEYBOARD_LAYOUT[row][col].c_str())) / 2,
                                   y + (keyHeight - M5.Display.fontHeight()) / 2);
                M5.Display.setTextColor(TFT_BLACK);
                M5.Display.setTextSize(3);
                M5.Display.print(NUMBERS_KEYBOARD_LAYOUT[row][col]);
            }
        }
    }

    String getNumberKeyFromTouch(int x, int y) {
        const int keyWidth = EPD_WIDTH / NUMBERS_KEYBOARD_COLS;
        const int keyHeight = 80;
        const int startY = EPD_HEIGHT - (NUMBERS_KEYBOARD_ROWS * keyHeight) - 60;

        if (y < startY) return "";

        int row = (y - startY) / keyHeight;
        int col = x / keyWidth;

        if (row >= 0 && row < NUMBERS_KEYBOARD_ROWS && col >= 0 && col < NUMBERS_KEYBOARD_COLS) {
            return NUMBERS_KEYBOARD_LAYOUT[row][col];
        }
        return "";
    }
}
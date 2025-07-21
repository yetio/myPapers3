#include "eng_keyboard.h"
#include "../ui.h"

namespace keyboards {
    KeyboardState currentKeyboardState = LOWERCASE;


    const String (*getCurrentLayout())[KEYBOARD_COLS] {
        switch (currentKeyboardState) {
            case UPPERCASE:
                return ENG_KEYBOARD_LAYOUT_UPPER;
            case SYMBOLS:
                return ENG_KEYBOARD_LAYOUT_SYMBOLS;
            case LOWERCASE:
            default:
                return ENG_KEYBOARD_LAYOUT_LOWER;
        }
    }

    void drawEngKeyboard() {
        const int keyWidth = EPD_WIDTH / KEYBOARD_COLS;
        const int keyHeight = 60;
        const int startY = EPD_HEIGHT - (KEYBOARD_ROWS * keyHeight) - 60;

        M5.Display.fillRect(0, startY, EPD_WIDTH, KEYBOARD_ROWS * keyHeight, TFT_WHITE);

        const String (*currentLayout)[KEYBOARD_COLS] = getCurrentLayout();
        for (int row = 0; row < KEYBOARD_ROWS; ++row) {
            for (int col = 0; col < KEYBOARD_COLS; ++col) {
                int x = col * keyWidth;
                int y = startY + row * keyHeight;

                M5.Display.setCursor(x + (keyWidth - M5.Display.textWidth(currentLayout[row][col].c_str())) / 2,
                                   y + (keyHeight - M5.Display.fontHeight()) / 2);
                M5.Display.setTextColor(TFT_BLACK);
                M5.Display.setTextSize(2);
                M5.Display.print(currentLayout[row][col]);
            }
        }
    }

    void toggleKeyboardState() {
        if (currentKeyboardState == LOWERCASE) {
            currentKeyboardState = UPPERCASE;
        } else if (currentKeyboardState == UPPERCASE) {
            currentKeyboardState = SYMBOLS;
        } else {
            currentKeyboardState = LOWERCASE;
        }
    }

    String getKeyFromTouch(int x, int y) {
        const int keyWidth = EPD_WIDTH / KEYBOARD_COLS;
        const int keyHeight = 60;
        const int startY = EPD_HEIGHT - (KEYBOARD_ROWS * keyHeight) - 60;

        if (y < startY) return "";

        int row = (y - startY) / keyHeight;
        int col = x / keyWidth;

        if (row >= 0 && row < KEYBOARD_ROWS && col >= 0 && col < KEYBOARD_COLS) {
            return getCurrentLayout()[row][col];
        }
        return "";
    }
}
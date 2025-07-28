#ifndef NUMBERS_KEYBOARD_H
#define NUMBERS_KEYBOARD_H

#include <M5Unified.h>
#include <String>

namespace keyboards {
    static const int NUMBERS_KEYBOARD_ROWS = 4;
    static const int NUMBERS_KEYBOARD_COLS = 3;
    
    const String NUMBERS_KEYBOARD_LAYOUT[NUMBERS_KEYBOARD_ROWS][NUMBERS_KEYBOARD_COLS] = {
        {"1", "2", "3"},
        {"4", "5", "6"},
        {"7", "8", "9"},
        {"<", "0", "."}
    };

    void drawNumbersKeyboard();
    String getNumberKeyFromTouch(int x, int y);
}

#endif
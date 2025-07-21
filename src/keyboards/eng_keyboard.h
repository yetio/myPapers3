#ifndef ENG_KEYBOARD_H
#define ENG_KEYBOARD_H

#include <M5Unified.h>
#include <String>

namespace keyboards {
    enum KeyboardState {
        LOWERCASE,
        UPPERCASE,
        SYMBOLS
    };


    static const int KEYBOARD_ROWS = 4;
    static const int KEYBOARD_COLS = 11;
    
    const String ENG_KEYBOARD_LAYOUT_LOWER[KEYBOARD_ROWS][KEYBOARD_COLS] = {
        {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "<"},
        {"q", "w", "e", "r", "t", "y", "u", "i", "o", "p", ">"},
        {"a", "s", "d", "f", "g", "h", "j", "k", "l", ".", "~"},
        {"z", "x", "c", "v", "b", "n", "m", ",", "?", " ", " "}
    };

    const String ENG_KEYBOARD_LAYOUT_UPPER[KEYBOARD_ROWS][KEYBOARD_COLS] = {
        {"!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "<"},
        {"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", ">"},
        {"A", "S", "D", "F", "G", "H", "J", "K", "L", ".", "~"},
        {"Z", "X", "C", "V", "B", "N", "M", ",", "?", " ", " "}
    };

    const String ENG_KEYBOARD_LAYOUT_SYMBOLS[KEYBOARD_ROWS][KEYBOARD_COLS] = {
        {"!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "<"},
        {"`", "~", "|", "(", ")", "{", "}", "[", "]", "\\", ">"},
        {";", ":", "'", "\"", "<", ">", ",", ".", "/", " ", "~"},
        {"-", "_", "=", "+", "*", "&", " ", ",", "?", " ", " "}
    };

    extern KeyboardState currentKeyboardState;
    const String (*getCurrentLayout())[KEYBOARD_COLS];

    void drawEngKeyboard();
    void toggleKeyboardState();
    String getKeyFromTouch(int x, int y);
}

#endif
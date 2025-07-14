#ifndef ENG_KEYBOARD_H
#define ENG_KEYBOARD_H

#include <M5Unified.h>
#include <String>

namespace keyboards {
    // Enum for keyboard state
    enum KeyboardState {
        LOWERCASE,
        UPPERCASE,
        SYMBOLS
    };

    // English keyboard layouts
    static const int KEYBOARD_ROWS = 4;
    static const int KEYBOARD_COLS = 11;
    
    const String ENG_KEYBOARD_LAYOUT_LOWER[KEYBOARD_ROWS][KEYBOARD_COLS] = { // Lowercase layout
        {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "<"},
        {"q", "w", "e", "r", "t", "y", "u", "i", "o", "p", ">"},
        {"a", "s", "d", "f", "g", "h", "j", "k", "l", ".", "~"}, // Added Shift '~'
        {"z", "x", "c", "v", "b", "n", "m", ",", "?", " ", " "}
    };

    const String ENG_KEYBOARD_LAYOUT_UPPER[KEYBOARD_ROWS][KEYBOARD_COLS] = { // Uppercase layout
        {"!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "<"}, // Symbols instead of numbers
        {"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", ">"},
        {"A", "S", "D", "F", "G", "H", "J", "K", "L", ".", "~"},
        {"Z", "X", "C", "V", "B", "N", "M", ",", "?", " ", " "}
    };

    const String ENG_KEYBOARD_LAYOUT_SYMBOLS[KEYBOARD_ROWS][KEYBOARD_COLS] = { // Symbols layout
        {"!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "<"},
        {"`", "~", "|", "(", ")", "{", "}", "[", "]", "\\", ">"},
        {";", ":", "'", "\"", "<", ">", ",", ".", "/", " ", "~"},
        {"-", "_", "=", "+", "*", "&", " ", ",", "?", " ", " "}
    };

    // Declare current keyboard state and layout
    extern KeyboardState currentKeyboardState;
    const String (*getCurrentLayout())[KEYBOARD_COLS]; // Function to get current layout

    // Function to draw the English keyboard
    void drawEngKeyboard();

    // Function to toggle keyboard state
    void toggleKeyboardState();

    // Function to get key from touch coordinates
    String getKeyFromTouch(int x, int y);
}

#endif // ENG_KEYBOARD_H
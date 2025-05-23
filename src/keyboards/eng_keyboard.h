#ifndef ENG_KEYBOARD_H
#define ENG_KEYBOARD_H

#include <M5Unified.h>
#include <vector>
#include <String>

namespace keyboards {
    // Enum for keyboard state
    enum KeyboardState {
        LOWERCASE,
        UPPERCASE,
        SYMBOLS
    };

    // English keyboard layouts
    const std::vector<std::vector<String>> ENG_KEYBOARD_LAYOUT_LOWER = { // Lowercase layout
        {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "<"},
        {"q", "w", "e", "r", "t", "y", "u", "i", "o", "p", ">"},
        {"a", "s", "d", "f", "g", "h", "j", "k", "l", ".", "~"}, // Added Shift '~'
        {"z", "x", "c", "v", "b", "n", "m", ",", "?", " ", " "}
    };

    const std::vector<std::vector<String>> ENG_KEYBOARD_LAYOUT_UPPER = { // Uppercase layout
        {"!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "<"}, // Symbols instead of numbers
        {"Q", "W", "E", "R", "T", "Y", "U", "I", "O", "P", ">"},
        {"A", "S", "D", "F", "G", "H", "J", "K", "L", ".", "~"},
        {"Z", "X", "C", "V", "B", "N", "M", ",", "?", " ", " "}
    };

    const std::vector<std::vector<String>> ENG_KEYBOARD_LAYOUT_SYMBOLS = { // Symbols layout
        {"!", "@", "#", "$", "%", "^", "&", "*", "(", ")", "<"},
        {"`", "~", "|", "(", ")", "{", "}", "[", "]", "\\", ">"},
        {";", ":", "'", "\"", "<", ">", ",", ".", "/", " ", "~"},
        {"-", "_", "=", "+", "*", "&", " ", ",", "?", " ", " "}
    };

    // Declare current keyboard state and layout
    extern KeyboardState currentKeyboardState;
    const std::vector<std::vector<String>>& getCurrentLayout(); // Function to get current layout

    // English keyboard layout
    const std::vector<std::vector<String>> ENG_KEYBOARD_LAYOUT = {
        {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "<"},
        {"q", "w", "e", "r", "t", "y", "u", "i", "o", "p", ">"},
        {"a", "s", "d", "f", "g", "h", "j", "k", "l", ".", " "},
        {"z", "x", "c", "v", "b", "n", "m", ",", "?", " ", " "}
    };

    // Function to draw the English keyboard
    void drawEngKeyboard();
}

#endif // ENG_KEYBOARD_H
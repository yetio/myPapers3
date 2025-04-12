#ifndef ENG_KEYBOARD_H
#define ENG_KEYBOARD_H

#include <M5Unified.h>
#include <vector>
#include <String>

namespace keyboards {
    // English keyboard layout
    const std::vector<std::vector<String>> ENG_KEYBOARD_LAYOUT = {
        {"1", "2", "3", "4", "5", "6", "7", "8", "9", "0", "<"},
        {"q", "w", "e", "r", "t", "y", "u", "i", "o", "p", "C"},
        {"a", "s", "d", "f", "g", "h", "j", "k", "l", ".", "↵"},
        {"z", "x", "c", "v", "b", "n", "m", ",", "?", " ", " "}
    };

    // Function to draw the English keyboard
    void drawEngKeyboard();
}

#endif // ENG_KEYBOARD_H
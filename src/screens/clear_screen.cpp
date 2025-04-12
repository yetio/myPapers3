// ./screens/clear_screen.cpp
#include "clear_screen.h"
#include "../ui.h"

namespace screens {
    void drawClearScreen() {
        // Clear the entire screen instantly
        M5.Display.fillScreen(TFT_WHITE);
        M5.Display.display();
    }
}
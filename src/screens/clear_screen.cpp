#include "clear_screen.h"
#include "../ui.h"

namespace screens {
    void drawClearScreen() {

        M5.Display.fillScreen(TFT_WHITE);
        M5.Display.display();
    }
}
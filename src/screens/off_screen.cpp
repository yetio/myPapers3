// ./screens/off_screen.cpp
#include "off_screen.h"
#include "../ui.h"

namespace screens {
    void drawOffScreen() {
        // Switch to CLEAR_SCREEN to clear everything
        ::currentScreen = CLEAR_SCREEN;
        ::renderCurrentScreen();

        // Write "Off screen" in the center
        ::bufferRow("Off screen", 2);

        // Draw buffered rows and display
        ::drawRowsBuffered();
        M5.Display.display();

        // Shutdown the device
        M5.Power.powerOff();
    }
}
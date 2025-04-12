// ./buttons/freeze.cpp
#include "freeze.h"
#include "../ui.h"
#include "../screens/txt_viewer_screen.h"
#include "../screens/img_viewer_screen.h"

void freezeAction() {
    displayMessage("Freeze");
    
    // Clear the screen
    M5.Display.fillScreen(TFT_WHITE);
    M5.Display.display();

    // Get the currently opened file and display it full screen
    if (currentScreen == TXT_VIEWER_SCREEN) {
        String currentFile = screens::getCurrentTxtFile();
        screens::displayFullScreenFile(currentFile);
    }
    else if (currentScreen == IMG_VIEWER_SCREEN) {
        String currentFile = screens::getCurrentImgFile();
        screens::displayFullScreenImgFile(currentFile);
    }

    // Ensure the display is updated before powering off
    M5.Display.display();

    // Shutdown the device to save power
    M5.Power.powerOff();
}
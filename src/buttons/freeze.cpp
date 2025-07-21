#include "freeze.h"
#include "../ui.h"
#include "../screens/txt_viewer_screen.h"
#include "../screens/img_viewer_screen.h"
#include <esp_sleep.h>

void freezeAction() {

    M5.Display.fillScreen(TFT_WHITE);
    M5.Display.display();


    if (currentScreen == TXT_VIEWER_SCREEN) {
        String currentFile = screens::getCurrentTxtFile();
        screens::displayFullScreenFile(currentFile);
    }
    else if (currentScreen == IMG_VIEWER_SCREEN) {
        String currentFile = screens::getCurrentImgFile();
        screens::displayFullScreenImgFile(currentFile);
    }


    M5.Display.display();

    M5.Power.powerOff();
    esp_deep_sleep_start();
}
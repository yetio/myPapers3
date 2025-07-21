#include "off_screen.h"
#include "../ui.h"
#include <esp_sleep.h>

namespace screens {
    void drawOffScreen() {

        ::currentScreen = CLEAR_SCREEN;
        ::renderCurrentScreen();


        ::bufferRow("Off screen", 2);


        ::drawRowsBuffered();
        M5.Display.display();


        WiFi.disconnect();


        SD.end();


        M5.Display.sleep();


        M5.Power.powerOff();


        esp_sleep_enable_timer_wakeup(0);
        esp_deep_sleep_start();
    }
}

// ./screens/off_screen.cpp
#include "off_screen.h"
#include "../ui.h"
#include <esp_sleep.h>

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

        // Disconnect from WiFi before shutting down
        WiFi.disconnect();

        // Deinitialize SD card
        SD.end();

        // Put display to sleep
        M5.Display.sleep();

        // Shutdown the device
        M5.Power.powerOff();

        // Фоллбэк: переводим в глубокий сон, если питание не было отключено (например, при подключении USB)
        esp_sleep_enable_timer_wakeup(0);
        esp_deep_sleep_start();
    }
}

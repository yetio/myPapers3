// ./screens/main_screen.cpp
#include "main_screen.h"
#include "../ui.h"
#include "../battery.h"
#include "../sdcard.h"
#include "../sd_gateway.h"
#include <WiFi.h> // Added for using WiFi

namespace screens {
    void drawMainScreen() {
        ::bufferRow("hi5stack", 2, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
        ::bufferRow("Voltage: " + String(getBatteryVoltage(), 2) + "V", 3, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
        ::bufferRow("SD Card: " + String(isSDCardMounted() ? "Mounted" : "Not Mounted"), 4, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);

        // Wi-Fi status row (row 5)
        if (WiFi.status() == WL_CONNECTED) {
            String ip = WiFi.localIP().toString();
            ::bufferRow("Wi-Fi: " + ip, 5, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true);
        } else {
            ::bufferRow("Wi-Fi: Not connected", 5, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true);
        }

        // SD Gateway row (row 6, кликабельная)
        // Обработка нажатия реализована в main.cpp через sd_gateway::toggleOrShow()
        String sdgwState = sd_gateway::isActive() ? "On" : "Off";
        ::bufferRow("SD Gateway: " + sdgwState, 6, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true);

        // TODO: локализация строк SD Gateway и Apps

        // Apps row (row 7, кликабельная)
        // Обработка нажатия реализована в main.cpp
        ::bufferRow("Apps", 7, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true);

        // Clear unused rows
        for (int row = 8; row <= 13; ++row) {
            ::bufferRow("", row, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
        }
    }
}
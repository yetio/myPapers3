// ./screens/main_screen.cpp
#include "main_screen.h"
#include "../ui.h"
#include "../battery.h"
#include "../sdcard.h"
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

        // Add Apps row (row 6)
        ::bufferRow("Apps", 6, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true); // Added Apps row

        // Clear unused rows
        for (int row = 7; row <= 13; ++row) { // Start clearing from row 7
            ::bufferRow("", row, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
        }
    }
}
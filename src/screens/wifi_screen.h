#ifndef WIFI_SCREEN_H
#define WIFI_SCREEN_H

#include <M5Unified.h>
#include <vector>
#include <String>
#include <WiFi.h>

namespace screens {
    void drawWifiScreen();
    void handleWiFiSelection(int row);
    void handleKeyboardInput(char key);
    extern bool isPasswordInputActive;
}

#endif // WIFI_SCREEN_H
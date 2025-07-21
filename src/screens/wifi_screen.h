#ifndef WIFI_SCREEN_H
#define WIFI_SCREEN_H

#include <M5Unified.h>
#include <functional>
#include <vector>
#include "../network/wifi_manager.h"

namespace screens {

    void drawWifiScreen();
    void handleWiFiScreenTouch(int x, int y);
    void resetWiFiScreen();

}

#endif

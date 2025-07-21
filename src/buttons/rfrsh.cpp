#include "rfrsh.h"
#include "../ui.h"
#include "../network/wifi_manager.h"

extern Message currentMessage;

void refreshUI() {
    displayMessage("Refresh pressed");

    if (currentScreen == WIFI_SCREEN) {
        WiFiManager::getInstance().startScan();
    } else if (currentScreen != TXT_VIEWER_SCREEN && currentScreen != IMG_VIEWER_SCREEN) {
        currentMessage.text = "";
    }

    renderCurrentScreen();
}
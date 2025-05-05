// ./buttons/off.cpp
#include "off.h"
#include "../ui.h"
#include "../screens/off_screen.h"

void showOffScreen() {
    displayMessage("Off pressed");
    footer.setVisible(false); // Hide footer
    clearMessage();
    screens::drawOffScreen();
    // Если устройство не выключилось физически (например, при USB), отправляем в глубокий сон
    esp_sleep_enable_timer_wakeup(0);
    esp_deep_sleep_start();
}
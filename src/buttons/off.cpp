#include "off.h"
#include "../ui.h"
#include "../screens/off_screen.h"
extern Message currentMessage;

void showOffScreen() {
    displayMessage("Off pressed");

    footer.setVisible(false);
    currentMessage.text = "";
    

    screens::drawOffScreen();
    

    esp_sleep_enable_timer_wakeup(0);
    esp_deep_sleep_start();
}
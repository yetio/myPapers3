#include "home.h"
#include "../ui.h"
#include "../screens/clear_screen.h"
#include "../screens/wifi_screen.h"
extern Message currentMessage;

void homeAction() {
    displayMessage("Home pressed");

    screens::drawClearScreen();
    

    screens::resetWiFiScreen();
    
    currentScreen = MAIN_SCREEN;
    

    currentMessage.text = "";
    

    renderCurrentScreen();
}
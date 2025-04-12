// ./buttons/off.cpp
#include "off.h"
#include "../ui.h"
#include "../screens/off_screen.h"

void showOffScreen() {
    displayMessage("Off pressed");
    footer.setVisible(false); // Hide footer
    clearMessage();
    screens::drawOffScreen();
}
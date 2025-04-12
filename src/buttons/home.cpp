// ./buttons/home.cpp
#include "home.h"
#include "../ui.h"
#include "../screens/clear_screen.h"

void homeAction() {
    displayMessage("Home pressed");
    screens::drawClearScreen(); // Clear the screen after message
    currentScreen = MAIN_SCREEN;
    clearMessage();
    renderCurrentScreen();
}
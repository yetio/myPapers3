// ./buttons/rfrsh.cpp
#include "rfrsh.h"
#include "../ui.h"

void refreshUI() {
    // Only refresh if current screen is not viewer screens
    if (currentScreen != TXT_VIEWER_SCREEN && currentScreen != IMG_VIEWER_SCREEN) {
        displayMessage("Refresh pressed");
        // No delay

        // Simply set the current screen to trigger re-render
        renderCurrentScreen();
        clearMessage();
    }
}
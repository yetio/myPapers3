// ./buttons/files.cpp
#include "files.h"
#include "../ui.h"
#include "../screens/clear_screen.h"
#include "../screens/files_screen.h" // Include files_screen.h for resetPagination

void filesAction() {
    displayMessage("Files pressed");
    screens::drawClearScreen(); // Clear the screen after message
    screens::resetPagination();  // Reset pagination when navigating to files
    currentPath = "/"; // Set to root directory
    currentScreen = FILES_SCREEN;
    renderCurrentScreen();
}
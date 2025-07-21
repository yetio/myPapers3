#include "files.h"
#include "../ui.h"
#include "../screens/clear_screen.h"
#include "../screens/files_screen.h"
extern Message currentMessage;

void filesAction() {
    displayMessage("Files pressed");

    screens::drawClearScreen();
    screens::resetPagination();
    currentPath = "/";
    currentScreen = FILES_SCREEN;
    

    currentMessage.text = "";
    renderCurrentScreen();
}
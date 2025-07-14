// ./buttons/files.cpp
#include "files.h"
#include "../ui.h"
#include "../screens/clear_screen.h"
#include "../screens/files_screen.h" // Include files_screen.h for resetPagination

// Объявляем внешнюю переменную для доступа к currentMessage
extern Message currentMessage;

void filesAction() {
    displayMessage("Files pressed");
    // Очищаем экран и переходим к файлам
    screens::drawClearScreen();
    screens::resetPagination();  // Reset pagination when navigating to files
    currentPath = "/"; // Set to root directory
    currentScreen = FILES_SCREEN;
    
    // Очищаем сообщение и выполняем рендер
    currentMessage.text = "";
    renderCurrentScreen();
}
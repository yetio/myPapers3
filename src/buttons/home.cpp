// ./buttons/home.cpp
#include "home.h"
#include "../ui.h"
#include "../screens/clear_screen.h"
#include "../screens/wifi_screen.h"

// Объявляем внешнюю переменную для доступа к currentMessage
extern Message currentMessage;

void homeAction() {
    displayMessage("Home pressed");
    // Очищаем экран и переходим на главный экран
    screens::drawClearScreen();
    
    // Сбрасываем состояние Wi-Fi экрана для предотвращения ошибок памяти
    screens::resetWiFiScreen();
    
    currentScreen = MAIN_SCREEN;
    
    // Очищаем сообщение без дополнительного рендера
    currentMessage.text = "";
    
    // Выполняем один рендер экрана
    renderCurrentScreen();
}
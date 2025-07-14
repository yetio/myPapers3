// ./buttons/off.cpp
#include "off.h"
#include "../ui.h"
#include "../screens/off_screen.h"

// Объявляем внешнюю переменную для доступа к currentMessage
extern Message currentMessage;

void showOffScreen() {
    displayMessage("Off pressed");
    // Скрываем футер и очищаем сообщения
    footer.setVisible(false);
    currentMessage.text = "";
    
    // Отображаем экран выключения
    screens::drawOffScreen();
    
    // Если устройство не выключилось физически (например, при USB), отправляем в глубокий сон
    esp_sleep_enable_timer_wakeup(0);
    esp_deep_sleep_start();
}
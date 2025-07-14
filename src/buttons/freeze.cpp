// ./buttons/freeze.cpp
#include "freeze.h"
#include "../ui.h"
#include "../screens/txt_viewer_screen.h"
#include "../screens/img_viewer_screen.h"
#include <esp_sleep.h>

void freezeAction() {
    // Очищаем экран
    M5.Display.fillScreen(TFT_WHITE);
    M5.Display.display();

    // Получаем текущий открытый файл и отображаем его на полный экран
    if (currentScreen == TXT_VIEWER_SCREEN) {
        String currentFile = screens::getCurrentTxtFile();
        screens::displayFullScreenFile(currentFile);
    }
    else if (currentScreen == IMG_VIEWER_SCREEN) {
        String currentFile = screens::getCurrentImgFile();
        screens::displayFullScreenImgFile(currentFile);
    }

    // Убеждаемся, что дисплей обновлен перед выключением
    M5.Display.display();

    // Выключаем устройство для экономии энергии
    M5.Power.powerOff();
    // Фоллбэк глубокий сон, если питание не отключилось при USB
    esp_deep_sleep_start();
}
#ifndef CONFIG_SCREEN_H
#define CONFIG_SCREEN_H

#include <Arduino.h>

namespace screens {
    void drawConfigScreen();
    void handleConfigTouch(int row, int x, int y);
}

#endif

#ifndef FILES_SCREEN_H
#define FILES_SCREEN_H

#include <M5Unified.h>

namespace screens {
    void drawFilesScreen();
    void handleTouch(int touchRow, int touchX, int touchY);
    void resetPagination();
}

#endif
#ifndef APPS_SCREEN_H
#define APPS_SCREEN_H

#include <M5Unified.h>
#include <SD.h>
#include <vector>
#include <String>

namespace screens {
    void drawAppsScreen();
    void handleAppsSelection(int selectedRow);
}

#endif
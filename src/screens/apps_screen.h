#ifndef APPS_SCREEN_H
#define APPS_SCREEN_H

#include <M5Unified.h>
#include <SD.h>
#include <vector>
#include <String>

namespace screens {
    void drawAppsScreen();
    void handleAppsSelection(int selectedRow); // Function to handle selection on Apps screen
}

#endif // APPS_SCREEN_H 
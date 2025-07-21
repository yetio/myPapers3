#ifndef TXT_VIEWER_SCREEN_H
#define TXT_VIEWER_SCREEN_H

#include <M5Unified.h>

namespace screens {
    void drawTxtViewerScreen(const String& filename);
    void displayTxtFile(const String& filename);
    void displayFullScreenFile(const String& filename);
    String getCurrentTxtFile();
    void setupFreezeButton();
    void setupTxtViewerRotateButtons();
}

#endif
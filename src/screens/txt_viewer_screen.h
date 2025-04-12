// ./screens/txt_viewer_screen.h
#ifndef TXT_VIEWER_SCREEN_H
#define TXT_VIEWER_SCREEN_H

#include <M5Unified.h>

namespace screens {
    void drawTxtViewerScreen(const String& filename);
    void displayTxtFile(const String& filename);
    void displayFullScreenFile(const String& filename); // For Freeze action
    String getCurrentTxtFile(); // For Freeze action
    void setupFreezeButton(); // Declare setupFreezeButton
}

#endif // TXT_VIEWER_SCREEN_H
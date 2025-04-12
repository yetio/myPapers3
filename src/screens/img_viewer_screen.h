// ./screens/img_viewer_screen.h
#ifndef IMG_VIEWER_SCREEN_H
#define IMG_VIEWER_SCREEN_H

#include <M5Unified.h>

namespace screens {
    void drawImgViewerScreen(const String& filename);
    void displayImgFile(const String& filename);
    void displayFullScreenImgFile(const String& filename); // Added for Freeze action
    String getCurrentImgFile();
    void setupImgViewerButtons();
}

#endif // IMG_VIEWER_SCREEN_H
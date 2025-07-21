#ifndef IMG_VIEWER_SCREEN_H
#define IMG_VIEWER_SCREEN_H

#include <M5Unified.h>

namespace screens {
    void drawImgViewerScreen(const String& filename);
    void displayImgFile(const String& filename);
    void displayFullScreenImgFile(const String& filename);
    String getCurrentImgFile();
    void setupImgViewerButtons();
    void setupImgViewerRotateButtons();
    void clearImgViewerScreen();
}

#endif
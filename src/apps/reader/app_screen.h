#ifndef READER_APP_SCREEN_H
#define READER_APP_SCREEN_H

#include <M5Unified.h>
#include <String>

namespace apps_reader {
    
    void drawAppScreen();
    
    
    void initApp();
    
    
    void handleTouch(int touchRow, int touchX, int touchY);
    
    
    void nextPage();
    void prevPage();
    
    
    void openFile(const String& filename);
    
    
    void returnToFileList();
    
    
    void ensureBooksFolder();
    
    
    bool isFileOpen();
    String getCurrentFile();
    int getCurrentPage();
    int getTotalPages();
}

#endif // READER_APP_SCREEN_H
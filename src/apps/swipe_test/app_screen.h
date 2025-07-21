#ifndef SWIPE_TEST_APP_SCREEN_H
#define SWIPE_TEST_APP_SCREEN_H

#include <M5Unified.h>
#include <String>

namespace apps_swipe_test {
    
    void drawAppScreen();
    
    
    void updateAnimation();
    
    
    void initApp();
    
    
    void handleTouch(int x, int y, bool isPressed);
    
    
    void handleSwipe(int startX, int startY, int endX, int endY);
    
    
    void resetState();
    

}

#endif // SWIPE_TEST_APP_SCREEN_H
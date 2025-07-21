#ifndef GEOMETRY_TEST_APP_SCREEN_H
#define GEOMETRY_TEST_APP_SCREEN_H

#include <M5Unified.h>
#include <String>

namespace apps_geometry_test {
    
    void drawAppScreen();
    
    
    void updateAnimation();
    
    
    void updateTimerOnly();
    
    
    void initApp();
    
    
    void toggleHorizontalLines();
    
    
    bool getHorizontalLinesState();
}

#endif // GEOMETRY_TEST_APP_SCREEN_H
// src/apps/geometry_test/app_screen.h
#ifndef GEOMETRY_TEST_APP_SCREEN_H
#define GEOMETRY_TEST_APP_SCREEN_H

#include <M5Unified.h>
#include <String>

namespace apps_geometry_test {
    // Функция для отображения экрана приложения
    void drawAppScreen();
    
    // Функция для обновления анимации (вызывается из основного цикла)
    void updateAnimation();
    
    // Функция для обновления только таймера
    void updateTimerOnly();
    
    // Функция для инициализации приложения
    void initApp();
    
    // Функция для переключения отображения горизонтальных линий
    void toggleHorizontalLines();
    
    // Функция для получения состояния отображения горизонтальных линий
    bool getHorizontalLinesState();
}

#endif // GEOMETRY_TEST_APP_SCREEN_H
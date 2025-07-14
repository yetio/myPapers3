// src/apps/swipe_test/app_screen.h
#ifndef SWIPE_TEST_APP_SCREEN_H
#define SWIPE_TEST_APP_SCREEN_H

#include <M5Unified.h>
#include <String>

namespace apps_swipe_test {
    // Функция для отображения экрана приложения
    void drawAppScreen();
    
    // Функция для обновления анимации (вызывается из основного цикла)
    void updateAnimation();
    
    // Функция для инициализации приложения
    void initApp();
    
    // Функция для обработки касаний и свайпов
    void handleTouch(int x, int y, bool isPressed);
    
    // Функция для обработки свайпа
    void handleSwipe(int startX, int startY, int endX, int endY);
    
    // Функция для сброса состояния
    void resetState();
    

}

#endif // SWIPE_TEST_APP_SCREEN_H
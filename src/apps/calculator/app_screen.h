#ifndef CALCULATOR_APP_SCREEN_H
#define CALCULATOR_APP_SCREEN_H

#include <M5Unified.h>
#include <String>

namespace apps_calculator {
    
    void drawAppScreen();
    
    void initApp();
    
    void handleTouch(int touchRow, int touchX, int touchY);
    
    void clearCalculator();
    
    void inputDigit(int digit);
    
    void inputOperation(char op);
    
    void calculate();
    
    void inputDecimal();
}

#endif // CALCULATOR_APP_SCREEN_H
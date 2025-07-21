#include "app_screen.h"
#include "../../ui.h"
#include <cmath>

namespace apps_calculator {
    const int WORK_AREA_X = 0;
    const int WORK_AREA_Y = 120;
    const int WORK_AREA_WIDTH = 540;
    const int WORK_AREA_BOTTOM = 845;
    
    const int DISPLAY_HEIGHT = 160;
    const int BUTTON_ROWS = 5;
    const int BUTTON_COLS = 4;
    const int BUTTON_WIDTH = WORK_AREA_WIDTH / BUTTON_COLS;
    const int BUTTON_HEIGHT = (WORK_AREA_BOTTOM - WORK_AREA_Y - DISPLAY_HEIGHT) / BUTTON_ROWS;
    
    static bool initialized = false;
    static String display = "0";
    static String expression = "";
    static double currentValue = 0;
    static double previousValue = 0;
    static char currentOperation = '\0';
    static bool waitingForOperand = false;
    static bool hasDecimal = false;
    static bool lastActionWasClear = false;
    
    const char buttons[BUTTON_ROWS][BUTTON_COLS] = {
        {'A', '/', '*', '-'},
        {'7', '8', '9', '+'},
        {'4', '5', '6', ' '},
        {'1', '2', '3', '='},
        {'0', '0', '.', ' '}
    };
    
    void initApp() {
        if (!initialized) {
            clearCalculator();
            initialized = true;
        }
    }
    
    void clearCalculator() {
        if (lastActionWasClear || expression == "0" || expression == "Error") {
            display = "0";
            expression = "0";
            currentValue = 0;
            previousValue = 0;
            currentOperation = '\0';
            waitingForOperand = false;
            hasDecimal = false;
            lastActionWasClear = false;
        } else {
            if (expression.length() > 1) {
                expression.remove(expression.length() - 1);
                if (expression.isEmpty()) {
                    expression = "0";
                }
                display = expression;
                lastActionWasClear = true;
            } else {
                expression = "0";
                display = "0";
                lastActionWasClear = true;
            }
        }
    }
    

    
    void drawCalculatorButton(char buttonChar, int row, int col) {
        int buttonX = WORK_AREA_X + col * BUTTON_WIDTH;
        int buttonY = WORK_AREA_Y + DISPLAY_HEIGHT + row * BUTTON_HEIGHT;
        
        int padding = 3;
        int actualX = buttonX + padding;
        int actualY = buttonY + padding;
        int actualWidth = BUTTON_WIDTH - 2 * padding;
        int actualHeight = BUTTON_HEIGHT - 2 * padding;
        
        if (row == 4 && col == 0 && buttonChar == '0') {
            actualWidth = BUTTON_WIDTH * 2 - 2 * padding;
        }
        
        if (buttonChar == '=' && row >= 2) {
            actualHeight = BUTTON_HEIGHT * 2 - 2 * padding;
        }
        
        if (buttonChar == '+' && row >= 1) {
            actualHeight = BUTTON_HEIGHT * 2 - 2 * padding;
        }
        
        uint16_t buttonColor = TFT_WHITE;
        uint16_t borderColor = TFT_BLACK;
        uint16_t textColor = TFT_BLACK;
        
        if (buttonChar == '=' || buttonChar == '+' || buttonChar == '-' || 
            buttonChar == '*' || buttonChar == '/') {
            buttonColor = TFT_LIGHTGREY;
        }
        
        if (buttonChar == 'A') {
            buttonColor = TFT_RED;
            textColor = TFT_WHITE;
        }
        
        M5.Display.fillRect(actualX, actualY, actualWidth, actualHeight, buttonColor);
        M5.Display.drawRect(actualX, actualY, actualWidth, actualHeight, borderColor);
        M5.Display.drawRect(actualX + 1, actualY + 1, actualWidth - 2, actualHeight - 2, borderColor);
        
        ::setUniversalFont();
        M5.Display.setTextColor(textColor, buttonColor);
        M5.Display.setTextSize(FONT_SIZE_ALL + 1);
        
        String buttonText = (buttonChar == 'A') ? "AC" : String(buttonChar);
        int textWidth = M5.Display.textWidth(buttonText.c_str());
        int textHeight = 30;
        int textX = actualX + (actualWidth - textWidth) / 2;
        int textY = actualY + (actualHeight - textHeight) / 2;
        
        M5.Display.setCursor(textX, textY);
        M5.Display.print(buttonText);
    }
    
    void drawAppScreen() {
        initApp();
        
        M5.Display.fillRect(WORK_AREA_X, WORK_AREA_Y, WORK_AREA_WIDTH, DISPLAY_HEIGHT, TFT_WHITE);
        M5.Display.drawRect(WORK_AREA_X, WORK_AREA_Y, WORK_AREA_WIDTH, DISPLAY_HEIGHT, TFT_BLACK);
        M5.Display.drawRect(WORK_AREA_X + 1, WORK_AREA_Y + 1, WORK_AREA_WIDTH - 2, DISPLAY_HEIGHT - 2, TFT_BLACK);
        
        ::setUniversalFont();
        M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
        M5.Display.setTextSize(10);
        
        String displayText = expression.isEmpty() ? display : expression;
        int textWidth = M5.Display.textWidth(displayText.c_str());
        int textX = WORK_AREA_X + WORK_AREA_WIDTH - textWidth - 20;
        int textY = WORK_AREA_Y + 20;
        M5.Display.setCursor(textX, textY);
        M5.Display.print(displayText);
        
        M5.Display.fillRect(WORK_AREA_X, WORK_AREA_Y + DISPLAY_HEIGHT, WORK_AREA_WIDTH, 
                           WORK_AREA_BOTTOM - WORK_AREA_Y - DISPLAY_HEIGHT, TFT_WHITE);
        
        for (int row = 0; row < BUTTON_ROWS; row++) {
            for (int col = 0; col < BUTTON_COLS; col++) {
                if (row == 4 && col == 1) continue;
                if (row == 4 && col == 3) continue;
                if (row == 2 && col == 3) continue;
                
                drawCalculatorButton(buttons[row][col], row, col);
            }
        }
    }
    
    void inputDigit(int digit) {
        lastActionWasClear = false;
        if (waitingForOperand) {
            display = String(digit);
            expression += String(digit);
            waitingForOperand = false;
            hasDecimal = false;
        } else {
            if (display == "0") {
                display = String(digit);
                if (expression == "0") {
                    expression = String(digit);
                } else {
                    expression += String(digit);
                }
            } else {
                if (display.length() < 12) {
                    display += String(digit);
                    expression += String(digit);
                }
            }
        }
        currentValue = display.toDouble();
    }
    
    void inputDecimal() {
        lastActionWasClear = false;
        if (waitingForOperand) {
            display = "0.";
            expression = "0.";
            waitingForOperand = false;
        } else if (!hasDecimal) {
            display += ".";
            expression += ".";
        }
        hasDecimal = true;
    }
    
    void inputOperation(char op) {
        lastActionWasClear = false;
        if (currentOperation != '\0' && !waitingForOperand) {
            calculate();
        }
        
        expression += op;
        previousValue = currentValue;
        currentOperation = op;
        waitingForOperand = true;
    }
    
    void calculate() {
        lastActionWasClear = false;
        if (currentOperation == '\0' || waitingForOperand) {
            return;
        }
        
        double result = previousValue;
        
        switch (currentOperation) {
            case '+':
                result = previousValue + currentValue;
                break;
            case '-':
                result = previousValue - currentValue;
                break;
            case '*':
                result = previousValue * currentValue;
                break;
            case '/':
                if (currentValue != 0) {
                    result = previousValue / currentValue;
                } else {
                    display = "Error";
                    expression = "Error";
                    clearCalculator();
                    return;
                }
                break;
        }
        
        currentValue = result;
        
        String resultStr;
        if (result == (long)result) {
            resultStr = String((long)result);
        } else {
            resultStr = String(result, 6);
            while (resultStr.endsWith("0") && resultStr.indexOf('.') != -1) {
                resultStr.remove(resultStr.length() - 1);
            }
            if (resultStr.endsWith(".")) {
                resultStr.remove(resultStr.length() - 1);
            }
        }
        
        expression += "=" + resultStr;
        display = resultStr;
        
        currentOperation = '\0';
        waitingForOperand = true;
        hasDecimal = display.indexOf('.') != -1;
    }
    
    void handleTouch(int touchRow, int touchX, int touchY) {
        if (touchY < WORK_AREA_Y + DISPLAY_HEIGHT || touchY > WORK_AREA_BOTTOM) {
            return;
        }
        
        int relativeY = touchY - (WORK_AREA_Y + DISPLAY_HEIGHT);
        int buttonRow = relativeY / BUTTON_HEIGHT;
        int buttonCol = touchX / BUTTON_WIDTH;
        
        if (buttonRow >= BUTTON_ROWS || buttonCol >= BUTTON_COLS) {
            return;
        }
        
        if (buttonRow == 4 && (buttonCol == 1 || buttonCol == 0)) {
            buttonCol = 0;
        }
        if (buttonRow >= 3 && buttonCol == 3) {
            buttonRow = 3;
            buttonCol = 3;
        }
        if ((buttonRow == 1 || buttonRow == 2) && buttonCol == 3) {
            buttonRow = 1;
            buttonCol = 3;
        }
        
        char buttonChar = buttons[buttonRow][buttonCol];
        
        switch (buttonChar) {
            case 'A':
                clearCalculator();
                break;
            case '0': case '1': case '2': case '3': case '4':
            case '5': case '6': case '7': case '8': case '9':
                inputDigit(buttonChar - '0');
                break;
            case '.':
                inputDecimal();
                break;
            case '+': case '-': case '*': case '/':
                inputOperation(buttonChar);
                break;
            case '=':
                calculate();
                break;
        }
        
        ::renderCurrentScreen();
    }
}
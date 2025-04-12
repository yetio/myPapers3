// button.cpp
#include "button.h"
#include "services/logger.h"

Button::Button(int x, int y, int width, int height, const String& label, std::function<void()> onPress)
    : _x(x), _y(y), _width(width), _height(height), _label(label), _onPress(onPress) {}

void Button::draw() const {
    Logger::getInstance().logButtonEvent("Drawing button: " + _label, 50.0);
    uint16_t btnColor = TFT_WHITE;
    uint16_t textColor = TFT_BLACK;

    // Draw button rectangle
    M5.Display.fillRect(_x, _y, _width, _height, btnColor);
    M5.Display.drawRect(_x, _y, _width, _height, TFT_BLACK);

    // Set text properties
    M5.Display.setTextColor(textColor, btnColor);
    M5.Display.setTextSize(3);
    int txtWidth = M5.Display.textWidth(_label.c_str());
    int txtX = _x + (_width - txtWidth) / 2;
    int txtY = _y + (_height - 24) / 2; // 24 - approximate text height

    // Draw button label
    M5.Display.setCursor(txtX, txtY);
    M5.Display.print(_label);
}

bool Button::isPressed(int touchX, int touchY) const {
    bool pressed = (touchX >= _x && touchX <= (_x + _width) &&
            touchY >= _y && touchY <= (_y + _height));
    if (pressed) {
        Logger::getInstance().logButtonEvent("Button pressed: " + _label, 25.0);
    }
    return pressed;
}

void Button::press() const {
    // Execute assigned action
    if (_onPress) {
        Logger::getInstance().logButtonEvent("Executing button action: " + _label, 75.0);
        _onPress();
    }
}

String Button::getLabel() const {
    return _label;
}
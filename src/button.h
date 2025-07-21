#ifndef BUTTON_H
#define BUTTON_H

#include <M5Unified.h>
#include <functional>

class Button {
public:
    Button(int x, int y, int width, int height, const String& label, std::function<void()> onPress);
    void draw() const;
    bool isPressed(int touchX, int touchY) const;
    void press() const;

    String getLabel() const;

private:
    int _x;
    int _y;
    int _width;
    int _height;
    String _label;
    std::function<void()> _onPress;
};

#endif
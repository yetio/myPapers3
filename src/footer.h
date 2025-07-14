// ./footer.h
#ifndef FOOTER_H
#define FOOTER_H

#include <M5Unified.h>
#include <String>
#include <functional>

// Forward declaration to avoid cyclic dependency
struct RowPosition;

// Constants for footer
const int MAX_FOOTER_BUTTONS = 4;

// Structure for footer buttons
struct FooterButton {
    String label;
    std::function<void()> action;
};

// Footer management
class Footer {
public:
    Footer();
    void setButtons(const FooterButton* buttons, int count);
    void draw(bool visible);
    void setVisible(bool visible);
    bool isVisible() const;
    void invokeButtonAction(int index);
    const FooterButton* getButtons() const { return buttons; }
    int getButtonCount() const { return buttonCount; }

private:
    FooterButton buttons[MAX_FOOTER_BUTTONS];
    int buttonCount;
    bool visible;
};

#endif // FOOTER_H
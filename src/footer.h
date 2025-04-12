// ./footer.h
#ifndef FOOTER_H
#define FOOTER_H

#include <M5Unified.h>
#include <String>
#include <vector>
#include <functional>

// Forward declaration to avoid cyclic dependency
struct RowPosition;

// Structure for footer buttons
struct FooterButton {
    String label;
    std::function<void()> action;
};

// Footer management
class Footer {
public:
    Footer();
    void setButtons(const std::vector<FooterButton>& buttons);
    void draw(bool visible);
    void setVisible(bool visible);
    bool isVisible() const;
    void invokeButtonAction(int index);
    const std::vector<FooterButton>& getButtons() const { return buttons; }

private:
    std::vector<FooterButton> buttons;
    bool visible;
};

#endif // FOOTER_H
#include "footer.h"
#include "ui.h"
#include <algorithm>
Footer::Footer() : buttonCount(0), visible(true) {}


void Footer::setButtons(const FooterButton* newButtons, int count) {
    buttonCount = (count > MAX_FOOTER_BUTTONS) ? MAX_FOOTER_BUTTONS : count;
    for (int i = 0; i < buttonCount; i++) {
        buttons[i] = newButtons[i];
    }
}


void Footer::draw(bool visible) {
    setVisible(visible);
    if (!visible) return;

    RowPosition pos = getRowPosition(15);
    int padding = 10;
    int availableWidth = pos.width - 2 * padding;
    if (buttonCount == 0) return;
    int buttonSpacing = 30;
    int totalSpacing = buttonSpacing * (buttonCount - 1);
    int buttonWidth = (availableWidth - totalSpacing) / buttonCount;
    
    int currentX = pos.x + padding;
    int textY = pos.y + 10;
    int underlineY = pos.y + 40;


    ::setUniversalFont();

    for (int i = 0; i < buttonCount; i++) {
        const FooterButton& button = buttons[i];

        M5.Display.setCursor(currentX, textY);
        M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
        M5.Display.setTextSize(FONT_SIZE_ALL);
        M5.Display.print(button.label);
        

        int textWidth = M5.Display.textWidth(button.label.c_str());
        M5.Display.drawLine(currentX, underlineY, currentX + textWidth, underlineY, TFT_BLACK);
        
        currentX += buttonWidth + buttonSpacing;
    }
}


void Footer::setVisible(bool vis) {
    visible = vis;
}


bool Footer::isVisible() const {
    return visible;
}


void Footer::invokeButtonAction(int index) {
    if (index >= 0 && index < buttonCount && buttons[index].action) {
        buttons[index].action();
    }
}
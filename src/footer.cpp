// ./footer.cpp
#include "footer.h"
#include "ui.h" // Include ui.h for RowPosition
#include <algorithm> // For std::max and std::min

// Initialize footer
Footer::Footer() : buttonCount(0), visible(true) {}

// Set footer buttons
void Footer::setButtons(const FooterButton* newButtons, int count) {
    buttonCount = (count > MAX_FOOTER_BUTTONS) ? MAX_FOOTER_BUTTONS : count;
    for (int i = 0; i < buttonCount; i++) {
        buttons[i] = newButtons[i];
    }
}

// Draw footer
void Footer::draw(bool visible) {
    setVisible(visible);
    if (!visible) return;

    RowPosition pos = getRowPosition(15); // Footer on row 15
    int padding = 10;
    int availableWidth = pos.width - 2 * padding;
    if (buttonCount == 0) return;
    int buttonSpacing = 30; // 5 spaces * ~6 pixels each
    int totalSpacing = buttonSpacing * (buttonCount - 1);
    int buttonWidth = (availableWidth - totalSpacing) / buttonCount;
    
    int currentX = pos.x + padding;
    int textY = pos.y + 10;
    int underlineY = pos.y + 40; // Увеличен отступ для правильного подчёркивания

    // Устанавливаем универсальный шрифт для футера
    ::setUniversalFont();

    for (int i = 0; i < buttonCount; i++) {
        const FooterButton& button = buttons[i];
        // Draw button label
        M5.Display.setCursor(currentX, textY);
        M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
        M5.Display.setTextSize(FONT_SIZE_ALL); // Используем общий размер шрифта
        M5.Display.print(button.label);
        
        // Calculate text width for underline
        int textWidth = M5.Display.textWidth(button.label.c_str());
        M5.Display.drawLine(currentX, underlineY, currentX + textWidth, underlineY, TFT_BLACK);
        
        currentX += buttonWidth + buttonSpacing;
    }
}

// Set footer visibility
void Footer::setVisible(bool vis) {
    visible = vis;
}

// Get footer visibility
bool Footer::isVisible() const {
    return visible;
}

// Invoke button action by index
void Footer::invokeButtonAction(int index) {
    if (index >= 0 && index < buttonCount && buttons[index].action) {
        buttons[index].action();
    }
}
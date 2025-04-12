// ./footer.cpp
#include "footer.h"
#include "ui.h" // Include ui.h for RowPosition
#include <algorithm> // For std::max and std::min

// Initialize footer
Footer::Footer() : visible(true) {}

// Set footer buttons
void Footer::setButtons(const std::vector<FooterButton>& newButtons) {
    buttons = newButtons;
}

// Draw footer
void Footer::draw(bool visible) {
    setVisible(visible);
    if (!visible) return;

    RowPosition pos = getRowPosition(15); // Footer on row 15
    int padding = 10;
    int availableWidth = pos.width - 2 * padding;
    int buttonCount = buttons.size();
    if (buttonCount == 0) return;
    int buttonSpacing = 30; // 5 spaces * ~6 pixels each
    int totalSpacing = buttonSpacing * (buttonCount - 1);
    int buttonWidth = (availableWidth - totalSpacing) / buttonCount;
    
    int currentX = pos.x + padding;
    int textY = pos.y + 10;
    int underlineY = pos.y + 30; // Adjust based on font size

    // Removed separator as per instruction

    for (const auto& button : buttons) {
        // Draw button label
        M5.Display.setCursor(currentX, textY);
        M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
        M5.Display.setTextSize(3);
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
    if (index >= 0 && index < buttons.size()) {
        buttons[index].action();
    }
}
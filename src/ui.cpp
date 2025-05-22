// ./ui.cpp
#include "ui.h"
#include <WiFi.h> // Added for using WiFi

// Флаг первого рендера: на нем - полное обновление, далее - быстрый DU4
static bool firstRenderDone = false;

// Initialize footer
Footer footer;

// Define global variables
Message currentMessage = {"", 0};
ScreenType currentScreen = MAIN_SCREEN;
String currentPath = "/";
std::vector<String> displayedFiles;
std::vector<BufferedRow> rowsBuffer;

// Row position structure
RowPosition getRowPosition(int row) {
    return RowPosition{0, row * 60, EPD_WIDTH, 60};
}

// Draw a single row on the screen with optional underline
void drawRow(const String& text, int row, uint16_t textColor, uint16_t bgColor, int fontSize, bool underline) {
    RowPosition pos = getRowPosition(row);
    M5.Display.fillRect(pos.x, pos.y, pos.width, pos.height, bgColor);
    M5.Display.setCursor(pos.x + 10, pos.y + 10);
    M5.Display.setTextColor(textColor, bgColor);
    M5.Display.setTextSize(fontSize);
    M5.Display.print(text);
    
    if (underline) {
        int textWidth = M5.Display.textWidth(text.c_str());
        int underlineY = pos.y + 30; // Adjust based on font size
        M5.Display.drawLine(pos.x + 10, underlineY, pos.x + 10 + textWidth, underlineY, TFT_BLACK);
    }
}

// Draw all rows from buffer at once
void drawRowsBuffered() {
    for (const auto& row : rowsBuffer) {
        bool isFooter = false; // Footer is handled separately
        drawRow(row.text, row.row, row.textColor, row.bgColor, row.fontSize, row.underline);
    }
    rowsBuffer.clear(); // Clear buffer after rendering
}

// Add row to buffer
void bufferRow(const String& text, int row, uint16_t textColor, uint16_t bgColor, int fontSize, bool underline) {
    rowsBuffer.emplace_back(BufferedRow{text, row, textColor, bgColor, fontSize, underline});
}

// Update the header with battery information
void updateHeader() {
    bufferRow("Battery: " + String(getBatteryPercentage()) + "%", 0, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
    if (currentMessage.text != "") {
        bufferRow(currentMessage.text, 1, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
    } else {
        bufferRow("", 1, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
    }
}

// Initialize UI
void setupUI() {
    // Setup initial screen elements
    currentScreen = MAIN_SCREEN;
    
    // Setup footer buttons for main screen
    std::vector<FooterButton> mainFooterButtons = {
        {"Home", homeAction},
        {"Off", showOffScreen},
        {"Rfrsh", refreshUI},
        {"Files", filesAction}
    };
    footer.setButtons(mainFooterButtons);
    
    renderCurrentScreen();
}

// Render the current screen with all elements
void renderCurrentScreen() {
    M5.Display.startWrite();

    rowsBuffer.clear();
    updateHeader();

    switch(currentScreen) {
        case MAIN_SCREEN: {
            std::vector<FooterButton> mainFooterButtons = {
                {"Home", homeAction},
                {"Off", showOffScreen},
                {"Rfrsh", refreshUI},
                {"Files", filesAction}
            };
            footer.setButtons(mainFooterButtons);
            screens::drawMainScreen();
            break;
        }
        case FILES_SCREEN: {
            std::vector<FooterButton> filesFooterButtons = {
                {"Home", homeAction},
                {"Off", showOffScreen},
                {"Rfrsh", refreshUI},
                {"Files", filesAction}
            };
            footer.setButtons(filesFooterButtons);
            screens::drawFilesScreen();
            break;
        }
        case OFF_SCREEN:
            screens::drawOffScreen();
            break;
        case TXT_VIEWER_SCREEN:
        case IMG_VIEWER_SCREEN: {
            std::vector<FooterButton> viewerFooterButtons = {
                {"Home", homeAction},
                {"Off", showOffScreen},
                {"Freeze", freezeAction},
                {"Files", filesAction}
            };
            footer.setButtons(viewerFooterButtons);
            if (currentScreen == TXT_VIEWER_SCREEN) {
                screens::drawTxtViewerScreen(currentPath);
            } else {
                screens::drawImgViewerScreen(currentPath);
            }
            break;
        }
        case CLEAR_SCREEN:
            screens::drawClearScreen();
            break;
        case WIFI_SCREEN: // Handle Wi-Fi screen
            screens::drawWifiScreen();
            break;
        default:
            break;
    }

    // Draw buffered rows
    drawRowsBuffered();

    // Draw footer
    footer.draw(footer.isVisible());

    M5.Display.endWrite();
    if (!firstRenderDone) {
        // Первое обновление - полное
        M5.Display.display();
        firstRenderDone = true;
    } else {
        // Частичное обновление региона контента (строки 2..15)
        RowPosition start = getRowPosition(2);
        RowPosition footerPos = getRowPosition(15);
        int regionY = start.y;
        int regionHeight = footerPos.y - start.y;
        M5.Display.display(start.x, regionY, start.width, regionHeight);
    }
}

// Update UI by clearing old messages
void updateUI() {
    if (currentMessage.text != "" && (millis() - currentMessage.timestamp > 1000)) { // 1 second
        clearMessage();
        renderCurrentScreen();
    }
}

// Display a message in header 2
void displayMessage(const String& msg) {
    currentMessage.text = msg;
    currentMessage.timestamp = millis();
    renderCurrentScreen();
}

// Clear the message in header 2
void clearMessage() {
    currentMessage.text = "";
    renderCurrentScreen();
}

// Navigate to a specific path
void navigateTo(const String& path) {
    currentPath = path;
    currentScreen = FILES_SCREEN;
    screens::resetPagination(); // Use namespace to call resetPagination
    
    // Setup footer buttons for files screen
    std::vector<FooterButton> filesFooterButtons = {
        {"Home", homeAction},
        {"Off", showOffScreen},
        {"Rfrsh", refreshUI},
        {"Files", filesAction}
    };
    footer.setButtons(filesFooterButtons);
    
    renderCurrentScreen();
}

// Navigate up a directory
void navigateUp() {
    if (currentPath == "/") return;

    while (currentPath.length() > 1 && currentPath.endsWith("/")) {
        currentPath = currentPath.substring(0, currentPath.length() - 1);
    }

    int lastSlash = currentPath.lastIndexOf('/');
    currentPath = (lastSlash > 0) ? currentPath.substring(0, lastSlash + 1) : "/";
    currentScreen = FILES_SCREEN;
    screens::resetPagination(); // Use namespace to call resetPagination
    
    // Setup footer buttons for files screen
    std::vector<FooterButton> filesFooterButtons = {
        {"Home", homeAction},
        {"Off", showOffScreen},
        {"Rfrsh", refreshUI},
        {"Files", filesAction}
    };
    footer.setButtons(filesFooterButtons);
    
    renderCurrentScreen();
}
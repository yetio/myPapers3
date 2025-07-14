// ./ui.cpp
#include "ui.h"
#include <WiFi.h> // Added for using WiFi

// Include app headers
#include "apps/text_lang_test/app_screen.h"
#include "apps/test2/app_screen.h"
#include "apps/geometry_test/app_screen.h"
#include "apps/swipe_test/app_screen.h"

// Флаг первого рендера: на нем - полное обновление, далее - быстрый DU4
bool firstRenderDone = false;

// Initialize footer
Footer footer;

// Define global variables
Message currentMessage = {"", 0};
ScreenType currentScreen = MAIN_SCREEN;
String currentPath = "/";
String displayedFiles[MAX_DISPLAYED_FILES];
int displayedFilesCount = 0;
BufferedRow rowsBuffer[MAX_ROWS_BUFFER];
int rowsBufferCount = 0;

// Функция для установки универсального шрифта
void setUniversalFont() {
    M5.Display.setFont(UNIVERSAL_FONT);
}

// Row position structure
RowPosition getRowPosition(int row) {
    return RowPosition{0, row * 60, EPD_WIDTH, 60};
}

// Get row number from Y coordinate
int getRowFromY(int y) {
    return y / 60;
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
        int underlineY = pos.y + 40; // Увеличен отступ для правильного подчёркивания
        M5.Display.drawLine(pos.x + 10, underlineY, pos.x + 10 + textWidth, underlineY, TFT_BLACK);
    }
}

// Draw all rows from buffer at once
void drawRowsBuffered() {
    for (int i = 0; i < rowsBufferCount; i++) {
        drawRow(rowsBuffer[i].text, rowsBuffer[i].row, rowsBuffer[i].textColor, 
                rowsBuffer[i].bgColor, rowsBuffer[i].fontSize, rowsBuffer[i].underline);
    }
    rowsBufferCount = 0; // Clear buffer after rendering
}

// Add row to buffer with size limit
void bufferRow(const String& text, int row, uint16_t textColor, uint16_t bgColor, int fontSize, bool underline) {
    // Ограничиваем размер буфера для предотвращения переполнения памяти
    if (rowsBufferCount >= MAX_ROWS_BUFFER) {
        // Очищаем весь буфер при переполнении для предотвращения ошибок памяти
        for (int i = 0; i < MAX_ROWS_BUFFER; i++) {
            rowsBuffer[i] = {"", 0, TFT_BLACK, TFT_WHITE, (int)FONT_SIZE_ALL, false};
        }
        rowsBufferCount = 0;
    }
    rowsBuffer[rowsBufferCount] = {text, row, textColor, bgColor, fontSize, underline};
    rowsBufferCount++;
}

// Force clear all memory buffers to prevent memory leaks
void clearAllBuffers() {
    rowsBufferCount = 0;
    displayedFilesCount = 0;
    // Очищаем содержимое массивов
    for (int i = 0; i < MAX_ROWS_BUFFER; i++) {
        rowsBuffer[i] = {"", 0, TFT_BLACK, TFT_WHITE, (int)FONT_SIZE_ALL, false};
    }
    for (int i = 0; i < MAX_DISPLAYED_FILES; i++) {
        displayedFiles[i] = "";
    }
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

// Статические массивы кнопок для предотвращения переполнения стека
static FooterButton mainFooterButtons[] = {
    {"Home", homeAction},
    {"Off", showOffScreen},
    {"Rfrsh", refreshUI},
    {"Files", filesAction}
};

static FooterButton filesFooterButtons[] = {
    {"Home", homeAction},
    {"Off", showOffScreen},
    {"Rfrsh", refreshUI},
    {"Files", filesAction}
};

static FooterButton viewerFooterButtons[] = {
    {"Home", homeAction},
    {"Off", showOffScreen},
    {"Freeze", freezeAction},
    {"Files", filesAction}
};

static FooterButton appFooterButtons[] = {
    {"Home", homeAction},
    {"Off", showOffScreen},
    {"", nullptr}, // Empty button for spacing
    {"", nullptr}  // Empty button for spacing
};

static FooterButton sdgwFooterButtons[] = {
    {"Home", homeAction},
    {"Off", showOffScreen},
    {"Rfrsh", refreshUI},
    {"Files", filesAction}
};

// Initialize UI
void setupUI() {
    // Устанавливаем универсальный шрифт
    setUniversalFont();
    
    // Setup initial screen elements
    currentScreen = MAIN_SCREEN;
    
    // Setup footer buttons for main screen using static array
    footer.setButtons(mainFooterButtons, 4);
    
    renderCurrentScreen();
}

// Render the current screen with all elements
void renderCurrentScreen() {
    M5.Display.startWrite();

    // Устанавливаем универсальный шрифт перед отрисовкой
    setUniversalFont();

    // Очищаем только rowsBuffer перед рендерингом
    rowsBufferCount = 0;
    updateHeader();

    // Clear content area before drawing
    RowPosition contentStart = getRowPosition(2);
    RowPosition footerStart = getRowPosition(15);
    M5.Display.fillRect(contentStart.x, contentStart.y, contentStart.width, footerStart.y - contentStart.y, TFT_WHITE);

    switch(currentScreen) {
        case MAIN_SCREEN:
            footer.setButtons(mainFooterButtons, 4);
            screens::drawMainScreen();
            break;
        case FILES_SCREEN:
            footer.setButtons(filesFooterButtons, 4);
            screens::drawFilesScreen();
            break;
        case OFF_SCREEN:
            screens::drawOffScreen();
            break;
        case TXT_VIEWER_SCREEN:
        case IMG_VIEWER_SCREEN:
            footer.setButtons(viewerFooterButtons, 4);
            if (currentScreen == TXT_VIEWER_SCREEN) {
                screens::drawTxtViewerScreen(currentPath);
            } else {
                screens::drawImgViewerScreen(currentPath);
            }
            break;
        case CLEAR_SCREEN:
            screens::drawClearScreen();
            break;
        case WIFI_SCREEN: // Handle Wi-Fi screen
            screens::drawWifiScreen();
            break;
        case APPS_SCREEN: // Handle Apps screen
            screens::drawAppsScreen();
            break;
        case TEXT_LANG_TEST_SCREEN: // Handle Text language font test app screen
            footer.setButtons(appFooterButtons, 4);
            apps_text_lang_test::drawAppScreen();
            break;
        case TEST2_APP_SCREEN: // Handle Test2 app screen
            footer.setButtons(appFooterButtons, 4);
            apps_test2::drawAppScreen();
            break;
        case GEOMETRY_TEST_SCREEN: // Handle Geometry Test app screen
            footer.setButtons(appFooterButtons, 4);
            apps_geometry_test::drawAppScreen();
            break;
        case SWIPE_TEST_SCREEN: // Handle Swipe Test app screen
            footer.setButtons(appFooterButtons, 4);
            apps_swipe_test::drawAppScreen();
            break;
        case SD_GATEWAY_SCREEN:
            footer.setButtons(sdgwFooterButtons, 4);
            screens::drawSdGatewayScreen();
            break;
    }

    // Draw all buffered rows
    drawRowsBuffered();

    // Draw footer
    footer.draw(footer.isVisible());

    M5.Display.endWrite();
}

// Update UI by clearing old messages
void updateUI() {
    // Clear message if it's too old
    clearMessage();

    // Update the display
    M5.Display.endWrite();
    M5.Display.display();

    // Reset rendering flag after display update
    extern bool isRendering;
    isRendering = false;
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

// Функция для переноса длинных строк текста
void wordWrap(const String& text, int maxWidth, String* lines, int& lineCount, int maxLines) {
    lineCount = 0;
    if (text.length() == 0) {
        return;
    }
    
    // Устанавливаем шрифт для корректного расчета ширины текста
    ::setUniversalFont();
    
    String line = "";
    String word = "";
    int spaceWidth = M5.Display.textWidth(" ");
    
    for (unsigned int i = 0; i < text.length(); i++) {
        char c = text[i];
        
        if (c == ' ' || c == '\n') {
            // Если текущая строка + слово + пробел шире максимальной ширины
            if (line.length() > 0 && 
                M5.Display.textWidth((line + word + " ").c_str()) > maxWidth) {
                if (lineCount < maxLines) {
                    lines[lineCount] = line;
                    lineCount++;
                }
                line = word + " ";
            } else {
                line += word + " ";
            }
            word = "";
            
            // Если встретили перенос строки, завершаем текущую строку
            if (c == '\n') {
                line.trim();
                if (lineCount < maxLines) {
                    lines[lineCount] = line;
                    lineCount++;
                }
                line = "";
            }
        } else {
            word += c;
        }
    }
    
    // Добавляем последнее слово
    if (word.length() > 0) {
        // Если текущая строка + последнее слово шире максимальной ширины
        if (line.length() > 0 && 
            M5.Display.textWidth((line + word).c_str()) > maxWidth) {
            if (lineCount < maxLines) {
                lines[lineCount] = line;
                lineCount++;
            }
            line = word;
        } else {
            line += word;
        }
    }
    
    // Добавляем последнюю строку, если она не пустая
    if (line.length() > 0) {
        line.trim();
        if (lineCount < maxLines) {
            lines[lineCount] = line;
            lineCount++;
        }
    }
}

// Navigate to a specific path
void navigateTo(const String& path) {
    currentPath = path;
    currentScreen = FILES_SCREEN;
    screens::resetPagination(); // Use namespace to call resetPagination
    
    // Setup footer buttons for files screen
    FooterButton filesFooterButtons[] = {
        {"Home", homeAction},
        {"Off", showOffScreen},
        {"Rfrsh", refreshUI},
        {"Files", filesAction}
    };
    footer.setButtons(filesFooterButtons, 4);
    
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
    FooterButton filesFooterButtons[] = {
        {"Home", homeAction},
        {"Off", showOffScreen},
        {"Rfrsh", refreshUI},
        {"Files", filesAction}
    };
    footer.setButtons(filesFooterButtons, 4);
    
    renderCurrentScreen();
}
#include "ui.h"
#include <WiFi.h>


#include "apps/text_lang_test/app_screen.h"
#include "apps/test2/app_screen.h"
#include "apps/geometry_test/app_screen.h"
#include "apps/swipe_test/app_screen.h"
#include "apps/calculator/app_screen.h"
#include "games/minesweeper/game.h"
#include "games/sudoku/game.h"
#include "games/test/game.h"
#include "screens/games_screen.h"


bool firstRenderDone = false;


Footer footer;


Message currentMessage = {"", 0};
ScreenType currentScreen = MAIN_SCREEN;
String currentPath = "/";
String displayedFiles[MAX_DISPLAYED_FILES];
int displayedFilesCount = 0;
BufferedRow rowsBuffer[MAX_ROWS_BUFFER];
int rowsBufferCount = 0;


void setUniversalFont() {
    M5.Display.setFont(UNIVERSAL_FONT);
}


RowPosition getRowPosition(int row) {
    return RowPosition{0, row * 60, EPD_WIDTH, 60};
}


int getRowFromY(int y) {
    return y / 60;
}


void drawRow(const String& text, int row, uint16_t textColor, uint16_t bgColor, int fontSize, bool underline) {
    RowPosition pos = getRowPosition(row);
    M5.Display.fillRect(pos.x, pos.y, pos.width, pos.height, bgColor);
    M5.Display.setCursor(pos.x + 10, pos.y + 10);
    M5.Display.setTextColor(textColor, bgColor);
    M5.Display.setTextSize(fontSize);
    M5.Display.print(text);
    
    if (underline) {
        int textWidth = M5.Display.textWidth(text.c_str());
        int underlineY = pos.y + 40;
        M5.Display.drawLine(pos.x + 10, underlineY, pos.x + 10 + textWidth, underlineY, TFT_BLACK);
    }
}


void drawRowsBuffered() {
    for (int i = 0; i < rowsBufferCount; i++) {
        drawRow(rowsBuffer[i].text, rowsBuffer[i].row, rowsBuffer[i].textColor, 
                rowsBuffer[i].bgColor, rowsBuffer[i].fontSize, rowsBuffer[i].underline);
    }
    rowsBufferCount = 0;
}


void bufferRow(const String& text, int row, uint16_t textColor, uint16_t bgColor, int fontSize, bool underline) {

    if (rowsBufferCount >= MAX_ROWS_BUFFER) {

        for (int i = 0; i < MAX_ROWS_BUFFER; i++) {
            rowsBuffer[i] = {"", 0, TFT_BLACK, TFT_WHITE, (int)FONT_SIZE_ALL, false};
        }
        rowsBufferCount = 0;
    }
    rowsBuffer[rowsBufferCount] = {text, row, textColor, bgColor, fontSize, underline};
    rowsBufferCount++;
}


void clearAllBuffers() {
    rowsBufferCount = 0;
    displayedFilesCount = 0;

    for (int i = 0; i < MAX_ROWS_BUFFER; i++) {
        rowsBuffer[i] = {"", 0, TFT_BLACK, TFT_WHITE, (int)FONT_SIZE_ALL, false};
    }
    for (int i = 0; i < MAX_DISPLAYED_FILES; i++) {
        displayedFiles[i] = "";
    }
}


void updateHeader() {
    bufferRow("Battery: " + String(getBatteryPercentage()) + "%", 0, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
    if (currentMessage.text != "") {
        bufferRow(currentMessage.text, 1, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
    } else {
        bufferRow("", 1, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
    }
}


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
    {"Rfrsh", refreshUI},
    {"Files", filesAction}
};

static FooterButton sdgwFooterButtons[] = {
    {"Home", homeAction},
    {"Off", showOffScreen},
    {"Rfrsh", refreshUI},
    {"Files", filesAction}
};


void setupUI() {

    setUniversalFont();
    

    currentScreen = MAIN_SCREEN;
    

    footer.setButtons(mainFooterButtons, 4);
    
    renderCurrentScreen();
}


void renderCurrentScreen() {
    M5.Display.startWrite();


    setUniversalFont();


    rowsBufferCount = 0;
    if (currentScreen != MINESWEEPER_GAME_SCREEN && 
        currentScreen != SUDOKU_GAME_SCREEN && 
        currentScreen != TEST_GAME_SCREEN) {
        updateHeader();
    }


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
        case WIFI_SCREEN:
            screens::drawWifiScreen();
            break;
        case APPS_SCREEN:
            screens::drawAppsScreen();
            break;
        case GAMES_SCREEN:
            drawGamesScreen();
            break;
        case TEXT_LANG_TEST_SCREEN:
            footer.setButtons(appFooterButtons, 4);
            apps_text_lang_test::drawAppScreen();
            break;
        case TEST2_APP_SCREEN:
            footer.setButtons(appFooterButtons, 4);
            apps_test2::drawAppScreen();
            break;
        case GEOMETRY_TEST_SCREEN:
            footer.setButtons(appFooterButtons, 4);
            apps_geometry_test::drawAppScreen();
            break;
        case SWIPE_TEST_SCREEN:
            footer.setButtons(appFooterButtons, 4);
            apps_swipe_test::drawAppScreen();
            break;
        case READER_APP_SCREEN:
            footer.setButtons(appFooterButtons, 4);
            apps_reader::drawAppScreen();
            break;
        case CALCULATOR_APP_SCREEN:
            footer.setButtons(appFooterButtons, 4);
            apps_calculator::drawAppScreen();
            break;
        case MINESWEEPER_GAME_SCREEN:
            games_minesweeper::drawGameScreen();
            break;
        case SUDOKU_GAME_SCREEN:
            games_sudoku::drawGameScreen();
            break;
        case TEST_GAME_SCREEN:
            games_test::drawGameScreen();
            break;
        case SD_GATEWAY_SCREEN:
            footer.setButtons(sdgwFooterButtons, 4);
            screens::drawSdGatewayScreen();
            break;
    }


    drawRowsBuffered();


    if (currentScreen != MINESWEEPER_GAME_SCREEN && 
        currentScreen != SUDOKU_GAME_SCREEN && 
        currentScreen != TEST_GAME_SCREEN) {
        footer.draw(footer.isVisible());
    }

    M5.Display.endWrite();
}


void updateUI() {

    clearMessage();


    M5.Display.endWrite();
    M5.Display.display();


    extern bool isRendering;
    isRendering = false;
}


void displayMessage(const String& msg) {
    currentMessage.text = msg;
    currentMessage.timestamp = millis();
    renderCurrentScreen();
}


void clearMessage() {
    currentMessage.text = "";
    renderCurrentScreen();
}


void wordWrap(const String& text, int maxWidth, String* lines, int& lineCount, int maxLines) {
    lineCount = 0;
    if (text.length() == 0) {
        return;
    }
    

    ::setUniversalFont();
    
    String line = "";
    String word = "";
    int spaceWidth = M5.Display.textWidth(" ");
    
    for (unsigned int i = 0; i < text.length(); i++) {
        char c = text[i];
        
        if (c == ' ' || c == '\n') {
    
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
    
    
    if (word.length() > 0) {

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
    

    if (line.length() > 0) {
        line.trim();
        if (lineCount < maxLines) {
            lines[lineCount] = line;
            lineCount++;
        }
    }
}


void navigateTo(const String& path) {
    currentPath = path;
    currentScreen = FILES_SCREEN;
    screens::resetPagination();
    

    FooterButton filesFooterButtons[] = {
        {"Home", homeAction},
        {"Off", showOffScreen},
        {"Rfrsh", refreshUI},
        {"Files", filesAction}
    };
    footer.setButtons(filesFooterButtons, 4);
    
    renderCurrentScreen();
}


void navigateUp() {
    if (currentPath == "/") return;

    while (currentPath.length() > 1 && currentPath.endsWith("/")) {
        currentPath = currentPath.substring(0, currentPath.length() - 1);
    }

    int lastSlash = currentPath.lastIndexOf('/');
    currentPath = (lastSlash > 0) ? currentPath.substring(0, lastSlash + 1) : "/";
    currentScreen = FILES_SCREEN;
    screens::resetPagination();
    

    FooterButton filesFooterButtons[] = {
        {"Home", homeAction},
        {"Off", showOffScreen},
        {"Rfrsh", refreshUI},
        {"Files", filesAction}
    };
    footer.setButtons(filesFooterButtons, 4);
    
    renderCurrentScreen();
}


void setCurrentScreen(ScreenType screen) {
    currentScreen = screen;
    renderCurrentScreen();
}
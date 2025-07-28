#ifndef UI_H
#define UI_H

#include <M5Unified.h>
#include "battery.h"
#include "sdcard.h"
#include "button.h"
#include "footer.h"
#include <String>
#include <WiFi.h> 


#include "screens/main_screen.h"
#include "screens/files_screen.h"
#include "screens/off_screen.h"
#include "screens/txt_viewer_screen.h"
#include "screens/img_viewer_screen.h"
#include "screens/clear_screen.h"
#include "screens/wifi_screen.h" 
#include "screens/sd_gateway_screen.h"
#include "screens/games_screen.h"


#include "buttons/home.h"
#include "buttons/off.h"
#include "buttons/rfrsh.h"
#include "buttons/files.h"
#include "buttons/freeze.h"
#include "buttons/rotate.h"


#include "apps/reader/app_screen.h"

const int EPD_WIDTH = 540;
const int EPD_HEIGHT = 960;
const float FONT_SIZE_ALL = 3;
const int ROW_HEIGHT_ALL = 60;


const lgfx::IFont* const UNIVERSAL_FONT = &fonts::efontCN_12;


extern bool firstRenderDone;


struct Message {
    String text;
    unsigned long timestamp;
};


struct RowPosition {
    int x;
    int y;
    int width;
    int height;
};


struct BufferedRow {
    String text;
    int row;
    uint16_t textColor;
    uint16_t bgColor;
    int fontSize;
    bool underline;
};


enum ScreenType {
    MAIN_SCREEN,
    FILES_SCREEN,
    OFF_SCREEN,
    TXT_VIEWER_SCREEN,
    IMG_VIEWER_SCREEN,
    CLEAR_SCREEN,
    WIFI_SCREEN,
    APPS_SCREEN,
    GAMES_SCREEN,
    TEXT_LANG_TEST_SCREEN,
    TEST2_APP_SCREEN,
    GEOMETRY_TEST_SCREEN,
    SWIPE_TEST_SCREEN,
    READER_APP_SCREEN,
    CALCULATOR_APP_SCREEN,
    MINESWEEPER_GAME_SCREEN,
    SUDOKU_GAME_SCREEN,
    TEST_GAME_SCREEN,
    SD_GATEWAY_SCREEN
};


const int MAX_DISPLAYED_FILES = 50;
const int MAX_ROWS_BUFFER = 25;


extern Message currentMessage;
extern ScreenType currentScreen;
extern String currentPath;
extern String displayedFiles[MAX_DISPLAYED_FILES];
extern int displayedFilesCount;
extern BufferedRow rowsBuffer[MAX_ROWS_BUFFER];
extern int rowsBufferCount;
extern Footer footer;


RowPosition getRowPosition(int row);
int getRowFromY(int y);
void drawRow(const String& text, int row, uint16_t textColor, uint16_t bgColor, int fontSize, bool underline = false);
void setupUI();
void updateUI();
void displayMessage(const String& msg);
void clearMessage();


void wordWrap(const String& text, int maxWidth, String* lines, int& lineCount, int maxLines);


void bufferRow(const String& text, int row, uint16_t textColor = TFT_BLACK, uint16_t bgColor = TFT_WHITE, int fontSize = FONT_SIZE_ALL, bool underline = false);
void drawRowsBuffered();
void renderCurrentScreen();
void clearAllBuffers();
void setCurrentScreen(ScreenType screen);


void setUniversalFont();


void navigateTo(const String& path);
void navigateUp();


void handleFileSelection(const String& selected);


namespace screens {
    void drawMainScreen();
    void drawFilesScreen();
    void drawOffScreen();
    void drawTxtViewerScreen(const String& filename);
    void drawImgViewerScreen(const String& filename);
    void drawWifiScreen();
    void displayFullScreenFile(const String& filename);
    void displayFullScreenImgFile(const String& filename);
    String getCurrentTxtFile();
    String getCurrentImgFile();
    void drawClearScreen();
    void startWiFiScan();
    void drawAppsScreen();
    void drawGamesScreen();

    void drawSdGatewayScreen();
    void handleSdGatewayAction();
}


void updateHeader();

#endif
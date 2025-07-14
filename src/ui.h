// ./ui.h
#ifndef UI_H
#define UI_H

#include <M5Unified.h>
#include "battery.h"
#include "sdcard.h"
#include "button.h"
#include "footer.h"
#include <String>
#include <WiFi.h> // Added

// Include new screen headers
#include "screens/main_screen.h"
#include "screens/files_screen.h"
#include "screens/off_screen.h"
#include "screens/txt_viewer_screen.h"
#include "screens/img_viewer_screen.h"
#include "screens/clear_screen.h"
#include "screens/wifi_screen.h" // Include Wi-Fi screen header
#include "screens/sd_gateway_screen.h"

// Include button action headers
#include "buttons/home.h"
#include "buttons/off.h"
#include "buttons/rfrsh.h"
#include "buttons/files.h"
#include "buttons/freeze.h"

const int EPD_WIDTH = 540;
const int EPD_HEIGHT = 960;
const float FONT_SIZE_ALL = 3;
const int ROW_HEIGHT_ALL = 60;

// Универсальный шрифт для всего проекта
const lgfx::IFont* const UNIVERSAL_FONT = &fonts::efontCN_12;

// Флаг для управления типом обновления экрана (полное/частичное)
extern bool firstRenderDone;

// Message structure
struct Message {
    String text;
    unsigned long timestamp;
};

// Row position structure
struct RowPosition {
    int x;
    int y;
    int width;
    int height;
};

// BufferedRow structure with underline flag
struct BufferedRow {
    String text;
    int row;
    uint16_t textColor;
    uint16_t bgColor;
    int fontSize;
    bool underline; // New member for underline
};

// Screen types
enum ScreenType {
    MAIN_SCREEN,
    FILES_SCREEN,
    OFF_SCREEN,
    TXT_VIEWER_SCREEN,
    IMG_VIEWER_SCREEN,
    CLEAR_SCREEN,
    WIFI_SCREEN, // Added WIFI_SCREEN
    APPS_SCREEN, // Added APPS_SCREEN
    TEXT_LANG_TEST_SCREEN, // Added TEXT_LANG_TEST_SCREEN
    TEST2_APP_SCREEN, // Added TEST2_APP_SCREEN
    SD_GATEWAY_SCREEN // Новый экран SD Gateway
};

// Constants for static arrays
const int MAX_DISPLAYED_FILES = 50;
const int MAX_ROWS_BUFFER = 25; // Увеличено для предотвращения переполнения буфера

// Declare global variables
extern Message currentMessage;
extern ScreenType currentScreen;
extern String currentPath;
extern String displayedFiles[MAX_DISPLAYED_FILES];
extern int displayedFilesCount;
extern BufferedRow rowsBuffer[MAX_ROWS_BUFFER];
extern int rowsBufferCount;
extern Footer footer;

// Declare functions
RowPosition getRowPosition(int row);
int getRowFromY(int y);
void drawRow(const String& text, int row, uint16_t textColor, uint16_t bgColor, int fontSize, bool underline = false); // Updated signature
void setupUI();
void updateUI();
void displayMessage(const String& msg);
void clearMessage();

// Функция для переноса длинных строк текста
void wordWrap(const String& text, int maxWidth, String* lines, int& lineCount, int maxLines);

// Added declarations
void bufferRow(const String& text, int row, uint16_t textColor = TFT_BLACK, uint16_t bgColor = TFT_WHITE, int fontSize = FONT_SIZE_ALL, bool underline = false); // Updated signature
void drawRowsBuffered();
void renderCurrentScreen();
void clearAllBuffers(); // Force clear all memory buffers

// Функция для установки универсального шрифта
void setUniversalFont();

// Navigation functions
void navigateTo(const String& path);
void navigateUp();

// Declare file selection handler
void handleFileSelection(const String& selected);

// External screen functions
namespace screens {
    void drawMainScreen();
    void drawFilesScreen();
    void drawOffScreen();
    void drawTxtViewerScreen(const String& filename);
    void drawImgViewerScreen(const String& filename);
    void drawWifiScreen(); // Include Wi-Fi screen header
    void displayFullScreenFile(const String& filename);
    void displayFullScreenImgFile(const String& filename);
    String getCurrentTxtFile();
    String getCurrentImgFile();
    void drawClearScreen();
    void startWiFiScan();
    void drawAppsScreen();
    // SD Gateway
    void drawSdGatewayScreen();
    void handleSdGatewayAction();
}

// Declare updateHeader
void updateHeader();

#endif // UI_H
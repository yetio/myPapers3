#include <algorithm> // For std::clamp, but if not available, define customClamp
#include <M5Unified.h>
#include <esp_sleep.h>
#include <WiFi.h>

#include "battery.h"
#include "sdcard.h"
#include "debug_config.h"
#include "ui.h"
#include "footer.h" // Include footer.h
#include "settings.h" // Include settings.h
#include "screens/wifi_screen.h" // Include Wi-Fi screen header
#include "screens/apps_screen.h" // Include Apps screen header
#include "keyboards/eng_keyboard.h" // Include English keyboard header
#include "sd_gateway.h"
#include "network/wifi_manager.h" // Include WiFi manager
#include "apps/text_lang_test/app_screen.h" // Include Text Language Test app header

bool isRendering = false;
bool ui_needs_update = true; // Флаг для управления обновлением UI
unsigned long lastTouchTime = 0;
const unsigned long TOUCH_DEBOUNCE_DELAY = 300; // Задержка дебаунсинга в миллисекундах

// Custom clamp function if std::clamp is unavailable
template <typename T>
T customClamp(T value, T minVal, T maxVal) {
    return (value < minVal) ? minVal : (value > maxVal) ? maxVal : value;
}

void setup() {
    M5.begin();
    M5.Display.begin();
    Serial.begin(115200);

    M5.Display.fillScreen(TFT_WHITE);
    M5.Display.setRotation(0);

    // Initialize SD card
    if (!initSDCard()) {
        Serial.println("Failed to initialize SD card");
    } else {
        Serial.println("SD card initialized successfully");
    }

    // Load settings
    Settings settings;
    WiFiSettings wifiSettings = settings.getWiFiSettings();

    // Проверка наличия SSID и пароля
    // if (wifiSettings.ssid != "" && wifiSettings.password != "") {
    //     // Попытка подключения к Wi-Fi только если пароль не пустой
    //     WiFi.begin(wifiSettings.ssid.c_str(), wifiSettings.password.c_str());
    //     if (WiFi.waitForConnectResult() == WL_CONNECTED) {
    //         Serial.println("Connected to " + wifiSettings.ssid);
    //     } else {
    //         Serial.println("Failed to connect to " + wifiSettings.ssid);
    //     }
    // } else {
    //     Serial.println("Wi-Fi credentials are missing or incomplete. Skipping auto-connect.");
    // }

    // Initialize UI
    setupUI();

    M5.Display.display();
}

void loop() {
    M5.update();
    if (ui_needs_update) {
        updateUI();
        ui_needs_update = false; // Сбрасываем флаг после обновления
    }
    sd_gateway::loop();
    
    // Проверяем завершение сканирования Wi-Fi
    WiFiManager::getInstance().updateScanResults();

    if (isRendering) {
        // Block actions during rendering
        return;
    }

    // Check for touch input with debouncing
    if (M5.Display.touch() && !isRendering && (millis() - lastTouchTime > TOUCH_DEBOUNCE_DELAY)) {
        lgfx::touch_point_t tp;
        if (M5.Display.getTouchRaw(&tp, 1)) {
            M5.Display.convertRawXY(&tp, 1);
            int16_t x = tp.x;
            int16_t y = tp.y;
            
            // Обновляем время последнего касания
            lastTouchTime = millis();
            isRendering = true;
            ui_needs_update = true; // Устанавливаем флаг для обновления UI
            
            // #ifdef DEBUG_TOUCH
            // Serial.print("Touch detected at x: ");
            // Serial.print(x);
            // Serial.print(", y: ");
            // Serial.println(y);
            // #endif

            // Handle footer touch
            if (footer.isVisible() && y >= (EPD_HEIGHT - 60)) { // Footer on row 15
                // Calculate which button was pressed based on x position
                int buttonCount = footer.getButtonCount();
                if (buttonCount == 0) return; // No buttons to handle

                int buttonWidth = EPD_WIDTH / buttonCount; // Dynamic based on button count
                int buttonIndex = x / buttonWidth;
                buttonIndex = customClamp(buttonIndex, 0, buttonCount - 1);

                if (buttonIndex < footer.getButtonCount()) {
                    #ifdef DEBUG_TOUCH
                    Serial.println(footer.getButtons()[buttonIndex].label + " button pressed");
                    #endif
                    footer.invokeButtonAction(buttonIndex);
                }
            } else {
                // Handle file selection if on FILES_SCREEN
                if (currentScreen == FILES_SCREEN) {
                    int touchedRow = getRowFromY(y);
                    screens::handleTouch(touchedRow, x, y);
                }
                // Handle Wi-Fi row touch on MAIN_SCREEN
                else if (currentScreen == MAIN_SCREEN) {
                    int touchedRow = getRowFromY(y);
                    if (touchedRow == 5) { // Wi-Fi row is row 5
                        displayMessage("Wi-Fi pressed");
                        screens::resetWiFiScreen(); // Сбрасываем состояние при входе на Wi-Fi экран
                        currentScreen = WIFI_SCREEN;
                        renderCurrentScreen();
                    } else if (touchedRow == 6) { // SD Gateway row is row 6
                        if (WiFi.status() != WL_CONNECTED) {
                            displayMessage("Wi-Fi не подключен");
                        } else {
                            sd_gateway::toggleOrShow();
                        }
                        renderCurrentScreen();
                    } else if (touchedRow == 7) { // Apps row is row 7
                        displayMessage("Apps pressed");
                        currentScreen = APPS_SCREEN;
                        renderCurrentScreen();
                    }
                }
                // Handle Wi-Fi screen touch
                else if (currentScreen == WIFI_SCREEN) {
                    screens::handleWiFiScreenTouch(x, y);
                }
                // Handle Apps screen touch
                else if (currentScreen == APPS_SCREEN) {
                    int touchedRow = getRowFromY(y);
                    if (touchedRow >= 3) { // App names start from row 3
                         screens::handleAppsSelection(touchedRow);
                    }
                }
            }
            

        }
        // Убрано постоянное сообщение "No touch data available" для предотвращения спама в консоли
    }
}

#include "wifi_screen.h"
#include "../ui.h"
#include "../settings.h"
#include "../keyboards/eng_keyboard.h"

namespace screens {
    static String selectedSSID = "";
    static String passwordInput = "";
    bool isPasswordInputActive = false;
    static bool isScanning = false;
    static unsigned long lastScanTime = 0;
    static unsigned long screenEntryTime = 0;
    static bool initialRefreshDone = false;
    static const unsigned long SCAN_INTERVAL = 10000; // 10 seconds between scans
    static int lastNetworkCount = 0;
    static std::vector<String> cachedNetworks;
    static std::vector<int> cachedSignalStrengths;

    void startWiFiScan() {
        if (!isScanning) {
            WiFi.mode(WIFI_STA);
            WiFi.disconnect(false);  // Disconnect but don't clear saved networks

            int result = WiFi.scanNetworks(true, true);  // Start async scan, show hidden networks
            if (result == WIFI_SCAN_FAILED) {
                result = WiFi.scanNetworks(true, true);
                if (result == WIFI_SCAN_FAILED) {
                    return;
                }
            }

            if (result != WIFI_SCAN_FAILED) {
                isScanning = true;
                lastScanTime = millis();
            }
        }
    }

    void updateNetworkList() {
        if (isScanning) {
            int result = WiFi.scanComplete();
            if (result >= 0) {
                // Scan completed successfully
                lastNetworkCount = result;
                cachedNetworks.clear();
                cachedSignalStrengths.clear();

                // Store all found networks
                int validNetworks = 0;
                for (int i = 0; i < result; ++i) {
                    // Skip empty SSIDs
                    if (WiFi.SSID(i).length() > 0) {
                        cachedNetworks.push_back(WiFi.SSID(i));
                        cachedSignalStrengths.push_back(WiFi.RSSI(i));
                        validNetworks++;
                    }
                }

                isScanning = false;
                WiFi.scanDelete(); // Clean up scan results
            }
        }
    }

    void drawWifiScreen() {
        if (WiFi.getMode() == WIFI_OFF) {
            WiFi.mode(WIFI_STA);
        }

        // Handle initial delay and refresh
        if (!initialRefreshDone) {
            if (screenEntryTime == 0) {
                screenEntryTime = millis();
                startWiFiScan();
            } else if (millis() - screenEntryTime >= 5000) { // 5 seconds delay
                initialRefreshDone = true;
            }
        }

        // Remove automatic periodic scan
        updateNetworkList();

        M5.Display.fillScreen(TFT_WHITE);

        if (isPasswordInputActive) {
            bufferRow("Enter password for: " + selectedSSID, 2, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true);
            bufferRow("Password: " + passwordInput, 3, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
            bufferRow("Cancel", 4, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true); // Clickable cancel button
            bufferRow("Enter", 5, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true); // Clickable enter button
            bufferRow("Test", 6, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true); // Clickable test button
            keyboards::drawEngKeyboard();
            drawRowsBuffered();
            {
                // Частичное обновление региона контента (строки 2..15)
                RowPosition start = getRowPosition(2);
                RowPosition footerPos = getRowPosition(15);
                int regionY = start.y;
                int regionHeight = footerPos.y - start.y;
                M5.Display.display(start.x, regionY, start.width, regionHeight);
            }
            return;
        }

        bufferRow("Wi-Fi Networks", 2, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true);

        if (isScanning) {
            bufferRow("Rfrsh", 3, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true); // Clickable refresh button
        } else if (cachedNetworks.empty()) {
            bufferRow("No networks found", 3, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
            startWiFiScan(); // Try to start a new scan
        } else {
            for (size_t i = 0; i < cachedNetworks.size(); ++i) {
                String signalStrength = String(cachedSignalStrengths[i]) + " dBm";
                bufferRow(cachedNetworks[i] + " - " + signalStrength, 3 + i, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
            }
        }

        drawRowsBuffered();
        footer.draw(footer.isVisible());
        {
            // Частичное обновление региона контента (строки 2..15)
            RowPosition start = getRowPosition(2);
            RowPosition footerPos = getRowPosition(15);
            int regionY = start.y;
            int regionHeight = footerPos.y - start.y;
            M5.Display.display(start.x, regionY, start.width, regionHeight);
        }
    }

    void handleWiFiSelection(int row) {
        if (isPasswordInputActive) {
            if (row == 4) { // Handle Cancel button click
                isPasswordInputActive = false;
                selectedSSID = "";
                passwordInput = "";
                renderCurrentScreen();
                return;
            } else if (row == 5) { // Handle Enter button click
                handleKeyboardInput("↵"); // Изменено на строковый литерал
                return;
            } else if (row == 6) { // Handle Test button click
                displayMessage("Test button pressed");
                return;
            }
        }
        
        if (row >= 3 && row < 3 + lastNetworkCount && !cachedNetworks.empty()) {
            selectedSSID = cachedNetworks[row - 3];
            isPasswordInputActive = true;
            passwordInput = "";
            renderCurrentScreen();
        }
    }

    void handleKeyboardInput(String key) { // Изменен тип переменной key на String
        Serial.print("Received key: ");
        Serial.println(key);

        if (key == "<") { // Изменено на строковый литерал
            if (passwordInput.length() > 0) {
                passwordInput.remove(passwordInput.length() - 1);
            }
        } else if (key == "C") { // Изменено на строковый литерал
            // Cancel button - return to WiFi networks list
            Serial.println("Cancel button pressed in handleKeyboardInput");
            displayMessage("Cancel button pressed");
            isPasswordInputActive = false;
            selectedSSID = "";
            passwordInput = "";
            renderCurrentScreen();
            return;
        } else if (key == "↵") { // Изменено на строковый литерал
            // Enter button - apply password and attempt connection
            if (!selectedSSID.isEmpty() && !passwordInput.isEmpty()) {
                Settings settings;
                settings.setWiFiSettings(selectedSSID, passwordInput);

                WiFi.disconnect();
                WiFi.begin(selectedSSID.c_str(), passwordInput.c_str());

                displayMessage("Connecting...");
                unsigned long startTime = millis();
                bool connected = false;

                while (millis() - startTime < 5000) { // 5 second timeout
                    if (WiFi.status() == WL_CONNECTED) {
                        connected = true;
                        break;
                    }
                    yield(); // неблокирующая пауза вместо delay
                }

                if (connected) {
                    displayMessage("Connected to " + selectedSSID);
                    currentScreen = MAIN_SCREEN;
                    screens::drawClearScreen();
                } else {
                    displayMessage("Connection failed");
                    WiFi.disconnect();
                }
            }

            // Return to WiFi networks list after connection attempt
            isPasswordInputActive = false;
            selectedSSID = "";
            passwordInput = "";
            renderCurrentScreen();
        } else {
            passwordInput += key;
        }

        // Update only the password display area
        if (isPasswordInputActive) {
            M5.Display.startWrite();
            RowPosition pos = getRowPosition(3);
            M5.Display.fillRect(pos.x, pos.y, pos.width, pos.height, TFT_WHITE);
            M5.Display.setCursor(pos.x + 10, pos.y + 10);
            M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
            M5.Display.setTextSize(FONT_SIZE_ALL);
            M5.Display.print("Password: " + passwordInput);
            M5.Display.endWrite();
            {
                // Частичное обновление региона контента (строки 2..15)
                RowPosition start = getRowPosition(2);
                RowPosition footerPos = getRowPosition(15);
                int regionY = start.y;
                int regionHeight = footerPos.y - start.y;
                M5.Display.display(start.x, regionY, start.width, regionHeight);
            }
        }
    }
}

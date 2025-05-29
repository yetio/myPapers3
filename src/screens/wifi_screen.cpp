#include "wifi_screen.h"
#include "../debug_config.h"
#include "../ui.h"
#include "../footer.h"
#include "../keyboards/eng_keyboard.h"
#include "../settings.h"

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

        // Add Wi-Fi status display
        String statusText = "Status: ";
        if (WiFi.status() == WL_CONNECTED) {
            statusText += "Connected (";
            statusText += WiFi.SSID();
            statusText += ")";
        } else if (WiFi.status() == WL_DISCONNECTED) {
            statusText += "Disconnected";
        } else if (isScanning) {
            statusText += "Scanning...";
        } else {
            statusText += "Idle";
        }
        bufferRow(statusText, 3, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);

        // Add Connect Last WiFi button
        Settings settings;
        String lastSSID = settings.getLastConnectedSSID();
        if (!lastSSID.isEmpty()) {
            bufferRow("Connect Last WiFi: " + lastSSID, 4, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true); // Clickable button
        }

        // Networks list always starts after status and optional Connect Last WiFi button
        int networksListStartRow = 4; // Always start list from row 4 if Connect Last WiFi button is not present, 5 if present.
        if (!lastSSID.isEmpty()) {
            networksListStartRow = 5; // Start list from row 5 if Connect Last WiFi button is present
        }

        if (cachedNetworks.empty()) {
            bufferRow("No WiFi found, press Rfrsh", networksListStartRow, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
            // Keep scanning logic here if needed, or rely solely on footer refresh
            if (!isScanning) { // Start scan if not already scanning and no networks found
                 startWiFiScan();
            }
        } else {
            for (size_t i = 0; i < cachedNetworks.size(); ++i) {
                String signalStrength = String(cachedSignalStrengths[i]) + " dBm";
                bufferRow(cachedNetworks[i] + " - " + signalStrength, networksListStartRow + i, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
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
                handleKeyboardInput(">"); // Изменено на строковый литерал
                return;
            } else if (row == 6) { // Handle Test button click
                displayMessage("Test button pressed");
                return;
            }
        }
        
        Settings settings;
        String lastSSID = settings.getLastConnectedSSID();
        // Recalculate networksListStartRow to handle touch correctly
        int networksListStartRow = 4; // Always start list from row 4 if Connect Last WiFi button is not present, 5 if present.
        if (!lastSSID.isEmpty()) {
            networksListStartRow = 5; // Start list from row 5 if Connect Last WiFi button is present
        }

        // Handle 'Connect Last WiFi' button click
        if (!lastSSID.isEmpty() && row == 4 && networksListStartRow == 5) { // Check if button is visible and clicked (adjusted row)
             displayMessage("Connecting to last network...");
             // Retrieve password and connect
             String lastPassword = settings.getLastConnectedPassword(); // Get saved password for last connected network
             if (!lastSSID.isEmpty() && !lastPassword.isEmpty()) { // Check if both SSID and password exist
                 WiFi.disconnect();
                 WiFi.begin(lastSSID.c_str(), lastPassword.c_str());

                 unsigned long startTime = millis();
                 bool connected = false;

                 while (millis() - startTime < 10000) { // Increased timeout for connection
                     if (WiFi.status() == WL_CONNECTED) {
                         connected = true;
                         break;
                     }
                     yield();
                 }

                 if (connected) {
                     displayMessage("Connected to " + lastSSID);
                     currentScreen = MAIN_SCREEN; // Navigate back to main screen on success
                     // No need to update last connected SSID here, it's already saved.
                 } else {
                     displayMessage("Connection failed");
                     WiFi.disconnect(); // Ensure disconnected on failure
                 }
             } else {
                 displayMessage("Last network credentials not found or incomplete.");
             }
             renderCurrentScreen(); // Re-render screen after attempt
             return;
        }

        // Handle network list selection
        if (row >= networksListStartRow && row < networksListStartRow + cachedNetworks.size() && !cachedNetworks.empty()) {
            selectedSSID = cachedNetworks[row - networksListStartRow]; // Adjust index based on start row
            isPasswordInputActive = true;
            passwordInput = "";
            renderCurrentScreen();
        }
    }

    void handleKeyboardInput(String key) { // Изменен тип переменной key на String
        #ifdef DEBUG_KEYBOARD
        Serial.print("Received key: ");
        Serial.println(key);
        #endif

        if (key == "<") { // Изменено на строковый литерал
            if (passwordInput.length() > 0) {
                passwordInput.remove(passwordInput.length() - 1);
            }
        } else if (key == "~") { // Handle Shift key
            // Toggle keyboard state
            if (keyboards::currentKeyboardState == keyboards::LOWERCASE) {
                keyboards::currentKeyboardState = keyboards::UPPERCASE;
            } else if (keyboards::currentKeyboardState == keyboards::UPPERCASE) {
                keyboards::currentKeyboardState = keyboards::SYMBOLS;
            } else {
                keyboards::currentKeyboardState = keyboards::LOWERCASE;
            }
            renderCurrentScreen(); // Re-render to show new keyboard layout
        } else if (key == "C") { // Изменено на строковый литерал
            // Cancel button - return to WiFi networks list
            #ifdef DEBUG_KEYBOARD
            Serial.println("Cancel button pressed in handleKeyboardInput");
            #endif
            displayMessage("Cancel button pressed");
            isPasswordInputActive = false;
            selectedSSID = "";
            passwordInput = "";
            renderCurrentScreen();
            return;
        } else if (key == ">") { // Changed from "↵" to ">" to match new Enter symbol
            // Enter button - apply password and attempt connection
            if (!selectedSSID.isEmpty() && !passwordInput.isEmpty()) {
                Settings settings;
                settings.setWiFiSettings(selectedSSID, passwordInput);
                // Update last connected SSID and Password on successful connection
                // Save last connected SSID and password
                settings.setLastConnectedSSID(selectedSSID); 
                settings.setLastConnectedPassword(passwordInput);

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
            
            // Draw "Password: " label
            M5.Display.setCursor(pos.x + 10, pos.y + 10); // Consistent left padding
            M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
            M5.Display.setTextSize(FONT_SIZE_ALL);
            M5.Display.print("Password: ");

            // Calculate the starting X position for the password input
            int labelWidth = M5.Display.textWidth("Password: ");
            int passwordInputX = pos.x + 10 + labelWidth; // Start after the label
            
            // Draw password input, aligned after the label
            M5.Display.setCursor(passwordInputX, pos.y + 10); 
            M5.Display.print(passwordInput);
            
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

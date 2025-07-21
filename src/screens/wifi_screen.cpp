#include "wifi_screen.h"
#include "../debug_config.h"
#include "../ui.h"
#include "../footer.h"
#include "../keyboards/eng_keyboard.h"
#include "../settings.h"

namespace screens {

    enum class WiFiScreenState { LIST, PASSWORD };
    static WiFiScreenState currentState = WiFiScreenState::LIST;
    static String selectedSSID = "";
    static String passwordInput = "";
    static int networksListStartRow = 4;

    void resetWiFiScreen() {
        currentState = WiFiScreenState::LIST;
        selectedSSID = "";
        passwordInput = "";
        

        WiFiManager::getInstance().setOnNetworkListUpdated([]() {
#ifdef DEBUG_WIFI_TOUCH
            Serial.println("WiFi scan completed, updating screen...");
#endif
            if (currentScreen == WIFI_SCREEN) {
#ifdef DEBUG_WIFI_TOUCH
                Serial.println("Rendering WiFi screen after scan");
#endif
                renderCurrentScreen();
            }
        });
        
        WiFiManager::getInstance().startScan();
    }

    void drawWifiScreen() {
        WiFiManager& wifiManager = WiFiManager::getInstance();
        wifiManager.updateScanResults();

        M5.Display.fillScreen(TFT_WHITE);

        if (currentState == WiFiScreenState::PASSWORD) {
            bufferRow("Enter password for: " + selectedSSID, 2, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true);
            bufferRow("Password: " + passwordInput, 3, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
            keyboards::drawEngKeyboard();
        } else {
            bufferRow("Wi-Fi Networks", 2, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true);

            String statusText = "Status: ";
            if (wifiManager.isConnected()) {
                statusText += "Connected (";
                statusText += WiFi.SSID();
                statusText += ")";
            } else if (wifiManager.isScanning()) {
                statusText += "Scanning...";
            } else {
                statusText += "Disconnected";
            }
            bufferRow(statusText, 3, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);

            Settings settings;
            String lastSSID = settings.getLastConnectedSSID();
            networksListStartRow = 4;
            if (!lastSSID.isEmpty()) {
                bufferRow("Connect Last: " + lastSSID, 4, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true);
                networksListStartRow = 5;
            }

            const auto* networks = wifiManager.getNetworks();
            int networksCount = wifiManager.getNetworksCount();

            if (networksCount == 0 && !wifiManager.isScanning()) {
                bufferRow("No WiFi found, press Rfrsh", networksListStartRow, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
            } else {
                for (int i = 0; i < networksCount; ++i) {
                    String signalStrength = String(networks[i].rssi) + " dBm";
                    bufferRow(networks[i].ssid + " - " + signalStrength, networksListStartRow + i, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
                }
            }
        }

        drawRowsBuffered();
        footer.draw(footer.isVisible());
        M5.Display.display();
    }

    void handleKeyboardInput(String key) {
        if (key == "<") {
            if (passwordInput.length() > 0) {
                passwordInput.remove(passwordInput.length() - 1);
            }
        } else if (key == "~") {
            keyboards::toggleKeyboardState();
        } else if (key == ">") {
            if (!selectedSSID.isEmpty() && !passwordInput.isEmpty()) {
                displayMessage("Connecting...");
                if (WiFiManager::getInstance().connect(selectedSSID, passwordInput)) {
                    displayMessage("Connected to " + selectedSSID);
                    Settings settings;
                    settings.setLastConnectedSSID(selectedSSID);
                    settings.setLastConnectedPassword(passwordInput);
                    currentScreen = MAIN_SCREEN;
                    screens::drawClearScreen();
                } else {
                    displayMessage("Connection failed");
                    currentState = WiFiScreenState::LIST;
                }
            }
        } else {
            passwordInput += key;
        }
        renderCurrentScreen();
    }

    void handleWiFiScreenTouch(int x, int y) {
        if (y > EPD_HEIGHT - 60) {
            return;
        }

        if (currentState == WiFiScreenState::PASSWORD) {
            String key = keyboards::getKeyFromTouch(x, y);
            if (!key.isEmpty()) {
                handleKeyboardInput(key);
            }
            return;
        }


        Settings settings;
        String lastSSID = settings.getLastConnectedSSID();
        

        

        if (!lastSSID.isEmpty() && y >= 240 && y < 300) {
            String lastPassword = settings.getLastConnectedPassword();
            if (!lastSSID.isEmpty() && !lastPassword.isEmpty()) {
                displayMessage("Connecting to last...");
                if (WiFiManager::getInstance().connect(lastSSID, lastPassword)) {
                    displayMessage("Connected to " + lastSSID);
                    currentScreen = MAIN_SCREEN;
                    screens::drawClearScreen();
                } else {
                    displayMessage("Connection failed");
                }
            } else {
                displayMessage("Last network info missing");
            }
            renderCurrentScreen();
            return;
        }

        const auto* networks = WiFiManager::getInstance().getNetworks();
        int networksCount = WiFiManager::getInstance().getNetworksCount();
        

        int networksStartRow = lastSSID.isEmpty() ? 4 : 5;
        int networksStartY = networksStartRow * 60;
        

        if (y >= networksStartY && networksCount > 0) {

            int networkIndex = (y - networksStartY) / 60;
            
    #ifdef DEBUG_WIFI_TOUCH
            Serial.printf("WiFiScreenTouch: y=%d, networksStartY=%d, networksStartRow=%d, ROW_HEIGHT=60, index=%d, networksCount=%d\n", y, networksStartY, networksStartRow, networkIndex, networksCount);
            if (networkIndex >= 0 && networkIndex < networksCount) {
                Serial.printf("Selected network: %s\n", networks[networkIndex].ssid.c_str());
            }
    #endif

            if (networkIndex >= 0 && networkIndex < networksCount) {
                selectedSSID = networks[networkIndex].ssid;
                passwordInput = "";
                currentState = WiFiScreenState::PASSWORD;
                renderCurrentScreen();
            }
        }
    }
}

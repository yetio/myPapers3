#include "wifi_manager.h"

bool WiFiManager::connect(const String& ssid, const String& password) {
    if (ssid.isEmpty()) return false;

    WiFi.disconnect();
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid.c_str(), password.c_str());

    unsigned long startTime = millis();
    while (millis() - startTime < 5000) { // 5 second timeout
        if (WiFi.status() == WL_CONNECTED) {
            return true;
        }
        yield();  // Неблокирующая пауза
    }

    return false;
}

bool WiFiManager::disconnect() {
    if (WiFi.status() == WL_CONNECTED) {
        return WiFi.disconnect(true);
    }
    return true;
}

bool WiFiManager::startScan() {
    if (_isScanning) return false;

    WiFi.mode(WIFI_STA);
    WiFi.disconnect(true);
    delay(100); // Даем время на отключение

    int result = WiFi.scanNetworks(true, true);
    if (result == WIFI_SCAN_FAILED) {
        result = WiFi.scanNetworks(true, true);
    }

    if (result != WIFI_SCAN_FAILED) {
        _isScanning = true;
        return true;
    }

    return false;
}

int WiFiManager::updateScanResults() {
    if (!_isScanning) return -1;

    int result = WiFi.scanComplete();
    if (result >= 0) {
        _networksCount = 0;

        for (int i = 0; i < result && _networksCount < MAX_NETWORKS; ++i) {
            if (WiFi.SSID(i).length() > 0) {
                _networks[_networksCount].ssid = WiFi.SSID(i);
                _networks[_networksCount].rssi = WiFi.RSSI(i);
                _networksCount++;
            }
        }

        _isScanning = false;
        WiFi.scanDelete();

        if (_onNetworkListUpdated) {
            _onNetworkListUpdated();
        }

        return _networksCount;
    }

    return -1;
}
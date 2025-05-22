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
    WiFi.disconnect();
    return true;
}

bool WiFiManager::startScan() {
    if (_isScanning) return false;

    WiFi.mode(WIFI_STA);
    WiFi.disconnect(false);

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
        _networks.clear();

        for (int i = 0; i < result; ++i) {
            if (WiFi.SSID(i).length() > 0) {
                _networks.push_back({WiFi.SSID(i), WiFi.RSSI(i)});
            }
        }

        _isScanning = false;
        WiFi.scanDelete();

        if (_onNetworkListUpdated) {
            _onNetworkListUpdated();
        }

        return _networks.size();
    }

    return -1;
}
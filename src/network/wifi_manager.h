#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
#include <vector>
#include <String>
#include <functional>

class WiFiManager {
public:
    struct NetworkInfo {
        String ssid;
        int32_t rssi;
    };

    static WiFiManager& getInstance() {
        static WiFiManager instance;
        return instance;
    }

    bool connect(const String& ssid, const String& password);
    bool disconnect();
    bool startScan();
    bool isScanning() const { return _isScanning; }
    bool isConnected() const { return WiFi.status() == WL_CONNECTED; }
    String getLocalIP() const { return WiFi.localIP().toString(); }
    const std::vector<NetworkInfo>& getNetworks() const { return _networks; }
    int updateScanResults();

    void setOnNetworkListUpdated(std::function<void()> callback) {
        _onNetworkListUpdated = callback;
    }

private:
    WiFiManager() = default;
    WiFiManager(const WiFiManager&) = delete;
    WiFiManager& operator=(const WiFiManager&) = delete;

    bool _isScanning = false;
    std::vector<NetworkInfo> _networks;
    std::function<void()> _onNetworkListUpdated;
};

#endif // WIFI_MANAGER_H
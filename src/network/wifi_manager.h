#ifndef WIFI_MANAGER_H
#define WIFI_MANAGER_H

#include <WiFi.h>
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
    static const int MAX_NETWORKS = 8;
    const NetworkInfo* getNetworks() const { return _networks; }
    int getNetworksCount() const { return _networksCount; }
    int updateScanResults();

    void setOnNetworkListUpdated(std::function<void()> callback) {
        _onNetworkListUpdated = callback;
    }

private:
    WiFiManager() = default;
    WiFiManager(const WiFiManager&) = delete;
    WiFiManager& operator=(const WiFiManager&) = delete;

    bool _isScanning = false;
    NetworkInfo _networks[MAX_NETWORKS];
    int _networksCount = 0;
    std::function<void()> _onNetworkListUpdated;
};

#endif
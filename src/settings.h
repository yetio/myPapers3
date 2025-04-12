#ifndef SETTINGS_H
#define SETTINGS_H

#include <ArduinoJson.h>
#include <SD.h>
#include <WiFi.h>

struct WiFiSettings {
    String ssid;
    String password;
};

class Settings {
public:
    Settings();
    bool loadSettings();
    bool saveSettings();
    WiFiSettings getWiFiSettings() const;
    void setWiFiSettings(const String& ssid, const String& password);

private:
    WiFiSettings _wifiSettings;
    const String _settingsFile = "/settings.json";

    bool _createDefaultSettings();
};

#endif // SETTINGS_H
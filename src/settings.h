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
    String getLastConnectedSSID() const;
    void setLastConnectedSSID(const String& ssid);
    String getLastConnectedPassword() const;
    void setLastConnectedPassword(const String& password);
    // new: Todoist token and iCal subscription URL
    String getTodoistToken() const;
    void setTodoistToken(const String& token);
    String getICalUrl() const;
    void setICalUrl(const String& url);

private:
    WiFiSettings _wifiSettings;
    String _lastConnectedSSID;
    String _lastConnectedPassword;
    String _todoistToken;
    String _icalUrl;
    const String _settingsFile = "/settings.json";

    bool _createDefaultSettings();
};

#endif // SETTINGS_H
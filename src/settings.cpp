#include "settings.h"

Settings::Settings() {
    if (!SD.exists(_settingsFile)) {
        _createDefaultSettings();
    }
    loadSettings();
}

bool Settings::loadSettings() {
    File file = SD.open(_settingsFile, FILE_READ);
    if (!file) {
        Serial.println("Failed to open settings file for reading.");
        return false;
    }

    StaticJsonDocument<512> doc;
    DeserializationError error = deserializeJson(doc, file);
    file.close();

    if (error) {
        Serial.println("Failed to parse settings file.");
        return false;
    }

    _wifiSettings.ssid = doc["wifi"]["ssid"].as<String>();
    _wifiSettings.password = doc["wifi"]["password"].as<String>();

    return true;
}

bool Settings::saveSettings() {
    File file = SD.open(_settingsFile, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to open settings file for writing.");
        return false;
    }

    StaticJsonDocument<512> doc;
    doc["wifi"]["ssid"] = _wifiSettings.ssid;
    doc["wifi"]["password"] = _wifiSettings.password;

    if (serializeJson(doc, file) == 0) {
        Serial.println("Failed to write to settings file.");
        file.close();
        return false;
    }

    file.close();
    return true;
}

WiFiSettings Settings::getWiFiSettings() const {
    return _wifiSettings;
}

void Settings::setWiFiSettings(const String& ssid, const String& password) {
    _wifiSettings.ssid = ssid;
    _wifiSettings.password = password;
    saveSettings();
}

bool Settings::_createDefaultSettings() {
    File file = SD.open(_settingsFile, FILE_WRITE);
    if (!file) {
        Serial.println("Failed to create settings file.");
        return false;
    }

    StaticJsonDocument<128> doc;
    doc["wifi"]["ssid"] = "";
    doc["wifi"]["password"] = "";

    if (serializeJson(doc, file) == 0) {
        Serial.println("Failed to write default settings.");
        file.close();
        return false;
    }

    file.close();
    return true;
}
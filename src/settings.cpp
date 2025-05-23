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

    // Check and add new fields if they don't exist (for backward compatibility)
    bool saveNeeded = false;
    if (!doc.containsKey("lastConnectedSSID")) {
        doc["lastConnectedSSID"] = "";
        saveNeeded = true;
    }
    if (!doc.containsKey("lastConnectedPassword")) {
        doc["lastConnectedPassword"] = "";
        saveNeeded = true;
    }

    _lastConnectedSSID = doc["lastConnectedSSID"].as<String>();
    _lastConnectedPassword = doc["lastConnectedPassword"].as<String>();

    if (saveNeeded) {
        // Close the current file handle before saving
        file.close();
        saveSettings(); // Save the updated document back to the file
    } else {
        file.close();
    }

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
    doc["lastConnectedSSID"] = _lastConnectedSSID;
    doc["lastConnectedPassword"] = _lastConnectedPassword;

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

String Settings::getLastConnectedSSID() const {
    return _lastConnectedSSID;
}

void Settings::setLastConnectedSSID(const String& ssid) {
    _lastConnectedSSID = ssid;
    saveSettings();
}

String Settings::getLastConnectedPassword() const {
    return _lastConnectedPassword;
}

void Settings::setLastConnectedPassword(const String& password) {
    _lastConnectedPassword = password;
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
    doc["lastConnectedSSID"] = "";
    doc["lastConnectedPassword"] = "";

    if (serializeJson(doc, file) == 0) {
        Serial.println("Failed to write default settings.");
        file.close();
        return false;
    }

    file.close();
    return true;
}
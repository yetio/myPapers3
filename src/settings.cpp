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

    if (doc.containsKey("todoist") && doc["todoist"].containsKey("token")) {
        _todoistToken = doc["todoist"]["token"].as<String>();
    } else {
        _todoistToken = "";
    }

    if (doc.containsKey("ical") && doc["ical"].containsKey("url")) {
        _icalUrl = doc["ical"]["url"].as<String>();
    } else {
        _icalUrl = "";
    }


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

        file.close();
        saveSettings();
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
    doc["todoist"]["token"] = _todoistToken;
    doc["ical"]["url"] = _icalUrl;

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

String Settings::getTodoistToken() const {
    return _todoistToken;
}

void Settings::setTodoistToken(const String& token) {
    _todoistToken = token;
    saveSettings();
}

String Settings::getICalUrl() const {
    return _icalUrl;
}

void Settings::setICalUrl(const String& url) {
    _icalUrl = url;
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
    doc["todoist"]["token"] = "";
    doc["ical"]["url"] = "";

    if (serializeJson(doc, file) == 0) {
        Serial.println("Failed to write default settings.");
        file.close();
        return false;
    }

    file.close();
    return true;
}
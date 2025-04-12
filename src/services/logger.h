#ifndef LOGGER_H
#define LOGGER_H

#include <Arduino.h>
#include <SD.h>
#include <String>
#include <vector>

class Logger {
public:
    static Logger& getInstance() {
        static Logger instance;
        return instance;
    }

    // Initialize logger with SD card
    bool init();

    // Log different types of events with power consumption
    void logWiFiEvent(const String& event, float powerConsumption);
    void logScreenEvent(const String& event, float powerConsumption);
    void logButtonEvent(const String& event, float powerConsumption);
    void logSDCardEvent(const String& event, float powerConsumption);
    void logSystemEvent(const String& event, float powerConsumption);

    // Generic log method
    void log(const String& category, const String& event, float powerConsumption);

    // Get total power consumption
    float getTotalPowerConsumption() const { return totalPowerConsumption; }

    // Get power consumption by category
    std::vector<std::pair<String, float>> getPowerConsumptionByCategory() const;

private:
    Logger() : totalPowerConsumption(0.0f) {}
    Logger(const Logger&) = delete;
    Logger& operator=(const Logger&) = delete;

    bool writeToFile(const String& message);
    String getCurrentTimestamp();

    float totalPowerConsumption;
    std::vector<std::pair<String, float>> categoryPowerConsumption;
};

#endif // LOGGER_H
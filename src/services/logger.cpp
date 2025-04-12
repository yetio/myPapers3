#include "logger.h"
#include <time.h>

bool Logger::init() {
    // Create logs directory if it doesn't exist
    if (!SD.exists("/logs")) {
        if (!SD.mkdir("/logs")) {
            return false;
        }
    }

    // Create logs.txt if it doesn't exist
    if (!SD.exists("/logs/logs.txt")) {
        File logFile = SD.open("/logs/logs.txt", FILE_WRITE);
        if (!logFile) {
            return false;
        }
        logFile.println(getCurrentTimestamp() + " [System] Logger initialized");
        logFile.close();
    }

    return true;
}

void Logger::logWiFiEvent(const String& event, float powerConsumption) {
    log("WiFi", event, powerConsumption);
}

void Logger::logScreenEvent(const String& event, float powerConsumption) {
    log("Screen", event, powerConsumption);
}

void Logger::logButtonEvent(const String& event, float powerConsumption) {
    log("Button", event, powerConsumption);
}

void Logger::logSDCardEvent(const String& event, float powerConsumption) {
    log("SDCard", event, powerConsumption);
}

void Logger::logSystemEvent(const String& event, float powerConsumption) {
    log("System", event, powerConsumption);
}

void Logger::log(const String& category, const String& event, float powerConsumption) {
    totalPowerConsumption += powerConsumption;

    // Update category-specific power consumption
    bool categoryFound = false;
    for (auto& pair : categoryPowerConsumption) {
        if (pair.first == category) {
            pair.second += powerConsumption;
            categoryFound = true;
            break;
        }
    }
    if (!categoryFound) {
        categoryPowerConsumption.push_back({category, powerConsumption});
    }

    // Format log message
    String message = getCurrentTimestamp() + " [" + category + "] " + event + 
                    " (Power: " + String(powerConsumption, 3) + "mW)";

    writeToFile(message);
}

bool Logger::writeToFile(const String& message) {
    File logFile = SD.open("/logs/logs.txt", FILE_APPEND);
    if (!logFile) {
        return false;
    }

    logFile.println(message);
    logFile.close();
    return true;
}

String Logger::getCurrentTimestamp() {
    time_t now;
    time(&now);
    char buf[sizeof "YYYY-MM-DD HH:MM:SS"];
    strftime(buf, sizeof buf, "%Y-%m-%d %H:%M:%S", localtime(&now));
    return String(buf);
}

std::vector<std::pair<String, float>> Logger::getPowerConsumptionByCategory() const {
    return categoryPowerConsumption;
}
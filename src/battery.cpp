// battery.cpp
#include "battery.h"
#include <M5Unified.h>
#include "services/logger.h"

int getBatteryPercentage() {
    float voltage = M5.Power.getBatteryVoltage() / 1000.0; // Convert mV to V
    int percentage = (voltage >= 4.15) ? 100 : (voltage <= 3.7) ? 0 : (int)((voltage - 3.7) * 100 / (4.15 - 3.7));
    
    Logger::getInstance().logSystemEvent("Battery percentage: " + String(percentage) + "%", 10.0);
    return percentage;
}

float getBatteryVoltage() {
    float voltage = M5.Power.getBatteryVoltage() / 1000.0; // Convert mV to V
    Logger::getInstance().logSystemEvent("Battery voltage: " + String(voltage, 2) + "V", 10.0);
    return voltage;
}
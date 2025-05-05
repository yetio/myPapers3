// battery.cpp
#include "battery.h"
#include <M5Unified.h>

int getBatteryPercentage() {
    float voltage = M5.Power.getBatteryVoltage() / 1000.0; // Convert mV to V
    int percentage = (voltage >= 4.15) ? 100 : (voltage <= 3.7) ? 0 : (int)((voltage - 3.7) * 100 / (4.15 - 3.7));
    
    return percentage;
}

float getBatteryVoltage() {
    float voltage = M5.Power.getBatteryVoltage() / 1000.0; // Convert mV to V
    return voltage;
}
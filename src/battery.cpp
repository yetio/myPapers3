// battery.cpp
#include "battery.h"
#include <M5Unified.h>

int getBatteryPercentage() {
    float voltage = M5.Power.getBatteryVoltage() / 1000.0; // Convert mV to V
    int percentage;
    
    if (voltage >= 4.0) {
        // 100% для напряжения от 4.0В и выше
        percentage = 100;
    } else if (voltage >= 3.9) {
        // 90-100% range
        percentage = 90 + (voltage - 3.9) * 10 / 0.1;
    } else if (voltage >= 3.8) {
        // 75-90% range
        percentage = 75 + (voltage - 3.8) * 15 / 0.1;
    } else if (voltage >= 3.7) {
        // 55-75% range
        percentage = 55 + (voltage - 3.7) * 20 / 0.1;
    } else if (voltage >= 3.6) {
        // 30-55% range
        percentage = 30 + (voltage - 3.6) * 25 / 0.1;
    } else if (voltage >= 3.5) {
        // 10-30% range
        percentage = 10 + (voltage - 3.5) * 20 / 0.1;
    } else if (voltage >= 3.4) {
        // 0-10% range (критически низкий заряд)
        percentage = (voltage - 3.4) * 10 / 0.1;
    } else {
        percentage = 0;
    }
    
    return percentage;
}

float getBatteryVoltage() {
    float voltage = M5.Power.getBatteryVoltage() / 1000.0; // Convert mV to V
    return voltage;
}
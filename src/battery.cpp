#include "battery.h"
#include <M5Unified.h>

int getBatteryPercentage() {
    float voltage = M5.Power.getBatteryVoltage() / 1000.0;
    int percentage;
    
    if (voltage >= 4.0) {

        percentage = 100;
    } else if (voltage >= 3.9) {

        percentage = 90 + (voltage - 3.9) * 10 / 0.1;
    } else if (voltage >= 3.8) {

        percentage = 75 + (voltage - 3.8) * 15 / 0.1;
    } else if (voltage >= 3.7) {

        percentage = 55 + (voltage - 3.7) * 20 / 0.1;
    } else if (voltage >= 3.6) {

        percentage = 30 + (voltage - 3.6) * 25 / 0.1;
    } else if (voltage >= 3.5) {

        percentage = 10 + (voltage - 3.5) * 20 / 0.1;
    } else if (voltage >= 3.4) {

        percentage = (voltage - 3.4) * 10 / 0.1;
    } else {
        percentage = 0;
    }
    
    return percentage;
}

float getBatteryVoltage() {
    float voltage = M5.Power.getBatteryVoltage() / 1000.0;
    return voltage;
}
// sdcard.h
#ifndef SDCARD_H
#define SDCARD_H

#include <M5Unified.h>
#include <SD.h>
#include <SPI.h>

// Пины для подключения SD-карты
#define SD_CS_PIN GPIO_NUM_47
#define SD_SCK_PIN GPIO_NUM_39
#define SD_MOSI_PIN GPIO_NUM_38
#define SD_MISO_PIN GPIO_NUM_40

// Глобальное состояние SD-карты
extern String sdCardStatus;

// Декларация функций
bool initSDCard();
bool isSDCardMounted();
void updateSDCardStatus();
void listFiles(const char* dirname);

#endif // SDCARD_H
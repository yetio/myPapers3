#ifndef DEBUG_CONFIG_H
#define DEBUG_CONFIG_H

// Конфигурация отладочных сообщений
// Раскомментируйте нужные строки для включения отладочных сообщений

// Отладка касаний и кнопок
// #define DEBUG_TOUCH

// Отладка клавиатуры
// #define DEBUG_KEYBOARD

// Отладка файловых операций
// #define DEBUG_FILES

// Отладка SD Gateway
// #define DEBUG_SD_GATEWAY

// Общая отладка (включает все вышеперечисленное)
// #define DEBUG_ALL

#ifdef DEBUG_ALL
    #define DEBUG_TOUCH
    #define DEBUG_KEYBOARD
    #define DEBUG_FILES
    #define DEBUG_SD_GATEWAY
#endif

#endif // DEBUG_CONFIG_H
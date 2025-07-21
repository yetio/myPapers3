# hi5stack

> **Note:** This project was created with the help of an LLM (vibe coding).

> **Warning:** This is a very early and raw version. There are many bugs, including a known issue where the battery drains even when the device is powered off.

**hi5stack** is a project for microcontroller devices based on M5Stack/M5Paper (ESP32), implemented using PlatformIO. The project is designed to create a modular user interface with support for file management, SD card, Wi-Fi, keyboard, and various screens.

## Architecture and Main Modules

### Core Modules
- **main.cpp** — Main application entry point with setup, loop, and touch handling
- **battery.[h/cpp]** — Battery voltage and percentage calculation functions
- **button.[h/cpp]** — Button class implementation with drawing and touch handling
- **footer.[h/cpp]** — Footer class implementation for bottom navigation buttons
- **sdcard.[h/cpp]** — SD card operations: reading, writing, presence check
- **settings.[h/cpp]** — Storage and management of user settings
- **ui.[h/cpp]** — Basic user interface functions
- **sd_gateway.[h/cpp]** — SD Gateway: web interface for uploading, deleting, batch deleting, and editing txt/json files on the SD card via browser
- **debug_config.h** — Debug configuration macros for various system components

### Applications (apps/)
- **geometry_test/** — Geometry test app with animated shapes and timer
- **reader/** — Text reader app with file list and pagination
- **swipe_test/** — Swipe gesture test app with touch tracking
- **test2/** — Simple test app displaying "Test2" text
- **text_lang_test/** — Multi-language text display test app

### Interface Components
- **buttons/** — Individual handlers for various interface buttons (home, files, freeze, off, refresh, rotate)
- **keyboards/** — Support for on-screen keyboards (English keyboard with layout switching)
- **screens/** — Interface screens: main, file manager, image viewer, text viewer, Wi-Fi, clear, power off, apps, SD Gateway
- **network/** — Wi-Fi connection management with scanning and connection features
- **services/** — Service modules (currently empty, reserved for future extensions)

## Key Features

- **Modular Architecture:** Clean separation by functional areas with extensible design
- **File System Support:** Complete SD card integration with file management capabilities
- **Multi-language Support:** Text rendering and display for various languages (English, Russian, Japanese, Chinese)
- **Built-in Applications:**
  - Text reader with pagination
  - Geometry test with animated shapes
  - Swipe gesture testing
  - Multi-language font testing
- **Network Capabilities:** Wi-Fi scanning, connection management, and web interface
- **User Interface:** Touch-based navigation with on-screen keyboards and footer buttons
- **Image Support:** BMP image viewing with rotation capabilities
- **Power Management:** Battery monitoring and power-off functionality
- **SD Gateway:** Complete web interface for file operations (upload, delete, batch operations, edit txt/json files)
- **Debug System:** Configurable debug output for different system components

## Platform

- **Hardware:** M5Stack/M5Paper (ESP32)
- **Development Environment:** PlatformIO

## Getting Started

1. Install PlatformIO.
2. Open the project in PlatformIO IDE.
3. Connect your M5Stack/M5Paper device.
4. Build and upload the project to the device.

## Repository Structure

```
├── .gitignore
├── .vscode/                    — Visual Studio Code settings
├── README.md                   — Project documentation
├── data/                       — Project data files
├── platformio.ini              — PlatformIO configuration
├── project_tree.md             — Detailed project structure
└── src/                        — Source code
    ├── apps/                   — Built-in applications
    │   ├── geometry_test/      — Animated shapes test
    │   ├── reader/             — Text file reader
    │   ├── swipe_test/         — Touch gesture testing
    │   ├── test2/              — Simple test application
    │   └── text_lang_test/     — Multi-language font test
    ├── buttons/                — Button action handlers
    ├── keyboards/              — On-screen keyboard implementations
    ├── network/                — Wi-Fi management
    ├── screens/                — UI screens (main, files, apps, etc.)
    ├── services/               — Service modules
    └── [core modules]          — Main system components
```

---

The project is easily extensible by adding new screens, services, and button handlers.

---

## Русский перевод

> **Примечание:** Этот проект создан с помощью LLM (vibe coding).

> **Внимание:** Это очень сырая версия. В проекте много багов, включая известную проблему — разряд батареи даже в выключенном состоянии.

**hi5stack** — это проект для микроконтроллерных устройств на базе M5Stack/M5Paper (ESP32), реализованный с использованием PlatformIO. Проект предназначен для создания модульного пользовательского интерфейса с поддержкой работы с файлами, SD-картой, Wi-Fi, клавиатурой и различными экранами.

## Архитектура и основные модули

### Основные модули
- **main.cpp** — Главная точка входа приложения с настройкой, циклом и обработкой касаний
- **battery.[h/cpp]** — Функции расчета напряжения и процента заряда батареи
- **button.[h/cpp]** — Реализация класса кнопок с отрисовкой и обработкой касаний
- **footer.[h/cpp]** — Реализация класса нижней панели для навигационных кнопок
- **sdcard.[h/cpp]** — Операции с SD-картой: чтение, запись, проверка наличия
- **settings.[h/cpp]** — Хранение и управление пользовательскими настройками
- **ui.[h/cpp]** — Базовые функции пользовательского интерфейса
- **sd_gateway.[h/cpp]** — SD Gateway: веб-интерфейс для загрузки, удаления, массового удаления и редактирования txt/json файлов на SD-карте
- **debug_config.h** — Макросы конфигурации отладки для различных компонентов системы

### Приложения (apps/)
- **geometry_test/** — Приложение тестирования геометрии с анимированными фигурами и таймером
- **reader/** — Приложение для чтения текста с списком файлов и пагинацией
- **swipe_test/** — Приложение тестирования жестов свайпа с отслеживанием касаний
- **test2/** — Простое тестовое приложение, отображающее текст "Test2"
- **text_lang_test/** — Приложение тестирования отображения многоязычного текста

### Компоненты интерфейса
- **buttons/** — Отдельные обработчики для различных кнопок интерфейса (home, files, freeze, off, refresh, rotate)
- **keyboards/** — Поддержка экранных клавиатур (английская клавиатура с переключением раскладок)
- **screens/** — Экраны интерфейса: главный, файловый менеджер, просмотр изображений, просмотр текста, Wi-Fi, очистка, выключение, приложения, SD Gateway
- **network/** — Управление Wi-Fi соединением со сканированием и функциями подключения
- **services/** — Сервисные модули (в настоящее время пустые, зарезервированы для будущих расширений)

## Ключевые возможности

### Модульная архитектура
- Четкое разделение на экраны, кнопки, приложения и сервисы
- Легко расширяемая структура для добавления новых функций
- Независимые модули для различных компонентов системы

### Поддержка файловой системы
- Полная поддержка операций с SD-картой (чтение, запись, удаление)
- Файловый менеджер с навигацией по директориям
- Поддержка различных форматов файлов (текст, изображения, JSON)

### Многоязычность
- Поддержка отображения текста на разных языках
- Специальное приложение для тестирования шрифтов и языков
- Настраиваемые языковые параметры

### Встроенные приложения
- **Чтение текста**: просмотр текстовых файлов с пагинацией
- **Тест геометрии**: анимированные геометрические фигуры с таймером
- **Тест свайпов**: отслеживание и визуализация жестов касания
- **Тест шрифтов**: проверка отображения многоязычного текста

### Сетевые возможности
- Управление Wi-Fi соединениями
- Сканирование доступных сетей
- Веб-интерфейс для удаленного доступа к файлам

### Пользовательский интерфейс
- Интуитивная навигация с сенсорным управлением
- Настраиваемые кнопки и экранные клавиатуры
- Адаптивный дизайн для различных размеров экрана

### Поддержка изображений
- Просмотр изображений с навигацией
- Поддержка различных графических форматов
- Масштабирование и позиционирование изображений

### Управление питанием
- Мониторинг уровня заряда батареи
- Расчет напряжения и процента заряда
- Функции энергосбережения

### SD Gateway
- Веб-интерфейс для доступа к файлам на SD-карте
- Загрузка, удаление и редактирование файлов через браузер
- Массовые операции с файлами
- Поддержка редактирования текстовых и JSON файлов

### Система отладки
- Настраиваемые макросы отладки для различных компонентов
- Детальное логирование операций
- Инструменты для диагностики и тестирования

## Платформа

- **Аппаратная основа:** M5Stack/M5Paper (ESP32)
- **Среда разработки:** PlatformIO

## Начало работы

1. Установите PlatformIO.
2. Откройте проект в PlatformIO IDE.
3. Подключите устройство M5Stack/M5Paper.
4. Соберите и загрузите проект на устройство.

## Структура репозитория

```
src/
├── apps/                           # Приложения
│   ├── geometry_test/              # Тест геометрии с анимацией
│   │   ├── app_screen.cpp          # Экран приложения с геометрическими фигурами
│   │   └── app_screen.h            # Заголовочный файл экрана
│   ├── reader/                     # Приложение для чтения текста
│   │   ├── app_screen.cpp          # Экран чтения с пагинацией
│   │   └── app_screen.h            # Заголовочный файл экрана
│   ├── swipe_test/                 # Тест жестов свайпа
│   │   ├── app_screen.cpp          # Экран тестирования касаний
│   │   └── app_screen.h            # Заголовочный файл экрана
│   ├── test2/                      # Простое тестовое приложение
│   │   ├── app_screen.cpp          # Базовый тестовый экран
│   │   └── app_screen.h            # Заголовочный файл экрана
│   └── text_lang_test/             # Тест многоязычного текста
│       ├── app_screen.cpp          # Экран тестирования шрифтов
│       └── app_screen.h            # Заголовочный файл экрана
├── buttons/                        # Обработчики кнопок интерфейса
│   ├── files_button.cpp            # Кнопка файлового менеджера
│   ├── freeze_button.cpp           # Кнопка заморозки экрана
│   ├── home_button.cpp             # Кнопка возврата на главный экран
│   ├── off_button.cpp              # Кнопка выключения
│   ├── refresh_button.cpp          # Кнопка обновления
│   └── rotate_button.cpp           # Кнопка поворота экрана
├── keyboards/                      # Экранные клавиатуры
│   ├── en_keyboard.cpp             # Английская клавиатура
│   └── en_keyboard.h               # Заголовочный файл клавиатуры
├── network/                        # Сетевые функции
│   ├── wifi_manager.cpp            # Управление Wi-Fi соединениями
│   └── wifi_manager.h              # Заголовочный файл Wi-Fi менеджера
├── screens/                        # Экраны пользовательского интерфейса
│   ├── apps_screen.cpp             # Экран списка приложений
│   ├── clear_screen.cpp            # Экран очистки
│   ├── files_screen.cpp            # Экран файлового менеджера
│   ├── image_screen.cpp            # Экран просмотра изображений
│   ├── main_screen.cpp             # Главный экран
│   ├── off_screen.cpp              # Экран выключения
│   ├── sd_gateway_screen.cpp       # Экран SD Gateway
│   ├── text_screen.cpp             # Экран просмотра текста
│   └── wifi_screen.cpp             # Экран настроек Wi-Fi
├── services/                       # Сервисные модули (зарезервировано)
├── battery.cpp                     # Функции мониторинга батареи
├── battery.h                       # Заголовочный файл батареи
├── button.cpp                      # Реализация класса кнопок
├── button.h                        # Заголовочный файл кнопок
├── debug_config.h                  # Конфигурация отладки
├── footer.cpp                      # Реализация нижней панели
├── footer.h                        # Заголовочный файл нижней панели
├── main.cpp                        # Главная точка входа приложения
├── sd_gateway.cpp                  # Реализация SD Gateway
├── sd_gateway.h                    # Заголовочный файл SD Gateway
├── sdcard.cpp                      # Операции с SD-картой
├── sdcard.h                        # Заголовочный файл SD-карты
├── settings.cpp                    # Управление настройками
├── settings.h                      # Заголовочный файл настроек
├── ui.cpp                          # Базовые функции интерфейса
└── ui.h                            # Заголовочный файл интерфейса
```

---

Проект легко расширяется за счёт добавления новых экранов, сервисов и обработчиков кнопок.

---

## 中文简介

> **注意：** 本项目由 LLM (vibe coding) 协助创建。

> **警告：** 这是非常早期和原始的版本，存在许多已知和未知的 bug，包括关机时电池仍然会放电的问题。

**hi5stack** 是一个基于 M5Stack/M5Paper (ESP32) 的微控制器项目，使用 PlatformIO 实现。该项目旨在创建一个模块化的用户界面，支持文件管理、SD 卡、Wi-Fi、屏幕键盘和多种界面。

## 架构与主要模块

### 核心模块
- **main.cpp** — 应用程序主入口点，包含设置、循环和触摸处理
- **battery.[h/cpp]** — 电池电压和电量百分比计算功能
- **button.[h/cpp]** — 按钮类实现，包含绘制和触摸处理
- **footer.[h/cpp]** — 导航按钮底部面板类实现
- **sdcard.[h/cpp]** — SD卡操作：读取、写入、存在检查
- **settings.[h/cpp]** — 用户设置的存储和管理
- **ui.[h/cpp]** — 用户界面基本功能
- **sd_gateway.[h/cpp]** — SD Gateway：用于上传、删除、批量删除和编辑SD卡上txt/json文件的Web界面
- **debug_config.h** — 系统各组件的调试配置宏

### 应用程序 (apps/)
- **geometry_test/** — 几何测试应用，包含动画图形和计时器
- **reader/** — 文本阅读应用，包含文件列表和分页功能
- **swipe_test/** — 滑动手势测试应用，包含触摸跟踪
- **test2/** — 简单测试应用，显示"Test2"文本
- **text_lang_test/** — 多语言文本显示测试应用

### 界面组件
- **buttons/** — 不同界面按钮的单独处理器（home、files、freeze、off、refresh、rotate）
- **keyboards/** — 屏幕键盘支持（英文键盘，支持布局切换）
- **screens/** — 界面屏幕：主屏幕、文件管理器、图像查看、文本查看、Wi-Fi、清理、关机、应用程序、SD Gateway
- **network/** — Wi-Fi连接管理，包含扫描和连接功能
- **services/** — 服务模块（目前为空，为未来扩展预留）

## 主要特性

### 模块化架构
- 屏幕、按钮、应用程序和服务的清晰分离
- 易于扩展的结构，便于添加新功能
- 系统各组件的独立模块

### 文件系统支持
- 完整的SD卡操作支持（读取、写入、删除）
- 带目录导航的文件管理器
- 支持多种文件格式（文本、图像、JSON）

### 多语言支持
- 支持不同语言的文本显示
- 专门的字体和语言测试应用
- 可配置的语言参数

### 内置应用程序
- **文本阅读**：带分页功能的文本文件查看
- **几何测试**：带计时器的动画几何图形
- **滑动测试**：触摸手势跟踪和可视化
- **字体测试**：多语言文本显示检查

### 网络功能
- Wi-Fi连接管理
- 可用网络扫描
- 文件远程访问的Web界面

### 用户界面
- 直观的触摸控制导航
- 可定制的按钮和屏幕键盘
- 适应不同屏幕尺寸的自适应设计

### 图像支持
- 带导航的图像查看
- 支持多种图形格式
- 图像缩放和定位

### 电源管理
- 电池电量监控
- 电压和电量百分比计算
- 节能功能

### SD Gateway
- 访问SD卡文件的Web界面
- 通过浏览器上传、删除和编辑文件
- 批量文件操作
- 支持文本和JSON文件编辑

### 调试系统
- 各组件的可配置调试宏
- 详细的操作日志记录
- 诊断和测试工具

## 平台

- **硬件：** M5Stack/M5Paper (ESP32)
- **开发环境：** PlatformIO

## 快速开始

1. 安装 PlatformIO。
2. 在 PlatformIO IDE 中打开项目。
3. 连接 M5Stack/M5Paper 设备。
4. 编译并上传项目到设备。

## 仓库结构

```
src/
├── apps/                           # 应用程序
│   ├── geometry_test/              # 几何动画测试
│   │   ├── app_screen.cpp          # 几何图形应用屏幕
│   │   └── app_screen.h            # 屏幕头文件
│   ├── reader/                     # 文本阅读应用
│   │   ├── app_screen.cpp          # 带分页的阅读屏幕
│   │   └── app_screen.h            # 屏幕头文件
│   ├── swipe_test/                 # 滑动手势测试
│   │   ├── app_screen.cpp          # 触摸测试屏幕
│   │   └── app_screen.h            # 屏幕头文件
│   ├── test2/                      # 简单测试应用
│   │   ├── app_screen.cpp          # 基础测试屏幕
│   │   └── app_screen.h            # 屏幕头文件
│   └── text_lang_test/             # 多语言文本测试
│       ├── app_screen.cpp          # 字体测试屏幕
│       └── app_screen.h            # 屏幕头文件
├── buttons/                        # 界面按钮处理器
│   ├── files_button.cpp            # 文件管理器按钮
│   ├── freeze_button.cpp           # 屏幕冻结按钮
│   ├── home_button.cpp             # 主屏幕返回按钮
│   ├── off_button.cpp              # 关机按钮
│   ├── refresh_button.cpp          # 刷新按钮
│   └── rotate_button.cpp           # 屏幕旋转按钮
├── keyboards/                      # 屏幕键盘
│   ├── en_keyboard.cpp             # 英文键盘
│   └── en_keyboard.h               # 键盘头文件
├── network/                        # 网络功能
│   ├── wifi_manager.cpp            # Wi-Fi连接管理
│   └── wifi_manager.h              # Wi-Fi管理器头文件
├── screens/                        # 用户界面屏幕
│   ├── apps_screen.cpp             # 应用程序列表屏幕
│   ├── clear_screen.cpp            # 清理屏幕
│   ├── files_screen.cpp            # 文件管理器屏幕
│   ├── image_screen.cpp            # 图像查看屏幕
│   ├── main_screen.cpp             # 主屏幕
│   ├── off_screen.cpp              # 关机屏幕
│   ├── sd_gateway_screen.cpp       # SD Gateway屏幕
│   ├── text_screen.cpp             # 文本查看屏幕
│   └── wifi_screen.cpp             # Wi-Fi设置屏幕
├── services/                       # 服务模块（预留）
├── battery.cpp                     # 电池监控功能
├── battery.h                       # 电池头文件
├── button.cpp                      # 按钮类实现
├── button.h                        # 按钮头文件
├── debug_config.h                  # 调试配置
├── footer.cpp                      # 底部面板实现
├── footer.h                        # 底部面板头文件
├── main.cpp                        # 应用程序主入口
├── sd_gateway.cpp                  # SD Gateway实现
├── sd_gateway.h                    # SD Gateway头文件
├── sdcard.cpp                      # SD卡操作
├── sdcard.h                        # SD卡头文件
├── settings.cpp                    # 设置管理
├── settings.h                      # 设置头文件
├── ui.cpp                          # 基础界面功能
└── ui.h                            # 界面头文件
```

---

该项目可通过添加新界面、服务和按钮处理器轻松扩展。

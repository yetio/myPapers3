# hi5stack

> **Note:** This project was created with the help of an LLM (vibe coding).

> **Warning:** This is a very early and raw version. There are many bugs, including a known issue where the battery drains even when the device is powered off.

**hi5stack** is a project for microcontroller devices based on M5Stack/M5Paper (ESP32), implemented using PlatformIO. The project is designed to create a modular user interface with support for file management, SD card, Wi-Fi, keyboard, and various screens.

## Architecture and Main Modules

- **main.cpp** — application entry point, system initialization, and main loop.
- **battery.[h/cpp]** — monitoring the device's battery status.
- **button.[h/cpp]** — basic handling of physical buttons.
- **footer.[h/cpp]** — displaying the interface footer panel.
- **sdcard.[h/cpp]** — SD card operations: reading, writing, presence check.
- **settings.[h/cpp]** — storage and management of user settings.
- **ui.[h/cpp]** — basic user interface functions.
- **sd_gateway.[h/cpp]** — SD Gateway: web interface for uploading, deleting, batch deleting, and editing txt/json files on the SD card via browser.

### Directories

- **buttons/** — individual handlers for various interface buttons (home, files, freeze, off, rfrsh).
- **keyboards/** — support for on-screen keyboards (e.g., English keyboard).
- **network/** — Wi-Fi connection management (wifi_manager).
- **screens/** — interface screens: main, file manager, image viewer, text viewer, Wi-Fi, clear, power off.
- **services/** — service modules, e.g., event logging (logger).

## Key Features

- Modular architecture with separation by functional areas.
- SD card and file system support.
- Wi-Fi and on-screen keyboard support.
- Multiple screens for different tasks (file viewing, image viewing, text viewing, Wi-Fi management).
- Event logging and handling.
- Support for control via physical and virtual buttons.
- **SD Gateway:** Web interface for uploading, deleting, batch deleting, and editing txt/json files directly from your browser.

## Platform

- **Hardware:** M5Stack/M5Paper (ESP32)
- **Development Environment:** PlatformIO

## Getting Started

1. Install PlatformIO.
2. Open the project in PlatformIO IDE.
3. Connect your M5Stack/M5Paper device.
4. Build and upload the project to the device.

## Repository Structure

- `src/` — application source code.
- `lib/` — third-party libraries (if used).
- `include/` — header files for global use.
- `test/` — tests.

---

The project is easily extensible by adding new screens, services, and button handlers.

---

## Русский перевод

> **Примечание:** Этот проект создан с помощью LLM (vibe coding).

> **Внимание:** Это очень сырая версия. В проекте много багов, включая известную проблему — разряд батареи даже в выключенном состоянии.

**hi5stack** — это проект для микроконтроллерных устройств на базе M5Stack/M5Paper (ESP32), реализованный с использованием PlatformIO. Проект предназначен для создания модульного пользовательского интерфейса с поддержкой работы с файлами, SD-картой, Wi-Fi, клавиатурой и различными экранами.

## Архитектура и основные модули

- **main.cpp** — точка входа приложения, инициализация системы и запуск основного цикла.
- **battery.[h/cpp]** — мониторинг состояния батареи устройства.
- **button.[h/cpp]** — базовая обработка физических кнопок.
- **footer.[h/cpp]** — отображение нижней панели интерфейса.
- **sdcard.[h/cpp]** — работа с SD-картой: чтение, запись, проверка наличия.
- **settings.[h/cpp]** — хранение и управление пользовательскими настройками.
- **ui.[h/cpp]** — базовые функции пользовательского интерфейса.
- **sd_gateway.[h/cpp]** — SD Gateway: веб-интерфейс для загрузки, удаления, массового удаления и редактирования txt/json файлов на SD-карте через браузер.

### Каталоги

- **buttons/** — отдельные обработчики для различных кнопок интерфейса (home, files, freeze, off, rfrsh).
- **keyboards/** — поддержка экранных клавиатур (например, английская клавиатура).
- **network/** — управление Wi-Fi соединением (wifi_manager).
- **screens/** — экраны интерфейса: главный, файловый менеджер, просмотр изображений, просмотр текста, Wi-Fi, очистка, выключение.
- **services/** — сервисные модули, например, логирование событий (logger).

## Ключевые возможности

- Модульная архитектура с разделением по функциональным областям.
- Работа с SD-картой и файловой системой.
- Поддержка Wi-Fi и экранных клавиатур.
- Несколько экранов для различных задач (просмотр файлов, изображений, текста, управление Wi-Fi).
- Логирование и обработка событий.
- Поддержка управления через физические и виртуальные кнопки.
- **SD Gateway:** Веб-интерфейс для загрузки, удаления, массового удаления и редактирования txt/json файлов прямо из браузера.

## Платформа

- **Аппаратная основа:** M5Stack/M5Paper (ESP32)
- **Среда разработки:** PlatformIO

## Начало работы

1. Установите PlatformIO.
2. Откройте проект в PlatformIO IDE.
3. Подключите устройство M5Stack/M5Paper.
4. Соберите и загрузите проект на устройство.

## Структура репозитория

- `src/` — исходный код приложения.
- `lib/` — сторонние библиотеки (если используются).
- `include/` — заголовочные файлы для глобального использования.
- `test/` — тесты.

---

Проект легко расширяется за счёт добавления новых экранов, сервисов и обработчиков кнопок.

---

## 中文简介

> **注意：** 本项目由 LLM (vibe coding) 协助创建。

> **警告：** 这是非常早期和原始的版本，存在许多已知和未知的 bug，包括关机时电池仍然会放电的问题。

**hi5stack** 是一个基于 M5Stack/M5Paper (ESP32) 的微控制器项目，使用 PlatformIO 实现。该项目旨在创建一个模块化的用户界面，支持文件管理、SD 卡、Wi-Fi、屏幕键盘和多种界面。

## 架构与主要模块

- **main.cpp** — 应用入口，系统初始化和主循环。
- **battery.[h/cpp]** — 设备电池状态监控。
- **button.[h/cpp]** — 物理按键处理。
- **footer.[h/cpp]** — 底部面板显示。
- **sdcard.[h/cpp]** — SD 卡操作：读写、检测。
- **settings.[h/cpp]** — 用户设置的存储与管理。
- **ui.[h/cpp]** — 基础 UI 功能。
- **sd_gateway.[h/cpp]** — SD Gateway：通过浏览器上传、删除、批量删除、编辑 txt/json 文件的网页界面。

### 目录结构

- **buttons/** — 各种界面按钮的独立处理器（home, files, freeze, off, rfrsh）。
- **keyboards/** — 屏幕键盘支持（如英文键盘）。
- **network/** — Wi-Fi 连接管理（wifi_manager）。
- **screens/** — 各种界面：主界面、文件管理器、图片查看器、文本查看器、Wi-Fi、清屏、关机。
- **services/** — 服务模块，如事件日志（logger）。

## 主要特性

- 按功能模块化架构。
- 支持 SD 卡和文件系统。
- 支持 Wi-Fi 和屏幕键盘。
- 多界面支持不同任务（文件、图片、文本查看，Wi-Fi 管理等）。
- 事件日志与处理。
- 支持物理和虚拟按钮控制。
- **SD Gateway：** 通过网页上传、删除、批量删除、直接编辑 txt/json 文件。

## 平台

- **硬件：** M5Stack/M5Paper (ESP32)
- **开发环境：** PlatformIO

## 快速开始

1. 安装 PlatformIO。
2. 在 PlatformIO IDE 中打开项目。
3. 连接 M5Stack/M5Paper 设备。
4. 编译并上传项目到设备。

## 仓库结构

- `src/` — 应用源代码。
- `lib/` — 第三方库（如有）。
- `include/` — 全局头文件。
- `test/` — 测试。

---

该项目可通过添加新界面、服务和按钮处理器轻松扩展。

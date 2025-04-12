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

## 日本語訳

> **注意:** このプロジェクトはLLM（vibe coding）を使って作成されました。

> **警告:** これは非常に初期で未完成のバージョンです。多くのバグがあり、特に電源オフ時にもバッテリーが消耗する既知の問題があります。

**hi5stack** は、PlatformIO を使用して実装された M5Stack/M5Paper（ESP32）ベースのマイコンデバイス向けプロジェクトです。ファイル管理、SDカード、Wi-Fi、キーボード、さまざまな画面をサポートするモジュラーUIの作成を目的としています。

## アーキテクチャと主なモジュール

- **main.cpp** — アプリケーションのエントリーポイント、システム初期化、メインループ。
- **battery.[h/cpp]** — デバイスのバッテリー状態の監視。
- **button.[h/cpp]** — 物理ボタンの基本的な処理。
- **footer.[h/cpp]** — インターフェースのフッターパネルの表示。
- **sdcard.[h/cpp]** — SDカードの操作：読み取り、書き込み、存在チェック。
- **settings.[h/cpp]** — ユーザー設定の保存と管理。
- **ui.[h/cpp]** — 基本的なユーザーインターフェース機能。

### ディレクトリ

- **buttons/** — 各種インターフェースボタン（home, files, freeze, off, rfrsh）の個別ハンドラ。
- **keyboards/** — オンスクリーンキーボードのサポート（例：英語キーボード）。
- **network/** — Wi-Fi接続管理（wifi_manager）。
- **screens/** — インターフェース画面：メイン、ファイルマネージャ、画像ビューア、テキストビューア、Wi-Fi、クリア、電源オフ。
- **services/** — サービスモジュール（例：イベントロギング logger）。

## 主な特徴

- 機能ごとに分離されたモジュラーアーキテクチャ。
- SDカードとファイルシステムのサポート。
- Wi-Fiとオンスクリーンキーボードのサポート。
- 複数の画面で様々なタスクに対応（ファイル閲覧、画像閲覧、テキスト閲覧、Wi-Fi管理）。
- イベントのロギングと処理。
- 物理ボタンと仮想ボタンによる操作サポート。

## プラットフォーム

- **ハードウェア:** M5Stack/M5Paper（ESP32）
- **開発環境:** PlatformIO

## はじめに

1. PlatformIOをインストールしてください。
2. PlatformIO IDEでプロジェクトを開きます。
3. M5Stack/M5Paperデバイスを接続します。
4. プロジェクトをビルドしてデバイスに書き込みます。

## リポジトリ構成

- `src/` — アプリケーションのソースコード。
- `lib/` — サードパーティ製ライブラリ（使用されている場合）。
- `include/` — グローバルに使用するヘッダファイル。
- `test/` — テスト。

---

新しい画面、サービス、ボタンハンドラを追加することで簡単に拡張できます。

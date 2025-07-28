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
- **calculator/** — Calculator app with basic arithmetic operations and AC functionality
- **geometry_test/** — Geometry test app with animated shapes and timer
- **reader/** — Text reader app with file list and pagination
- **swipe_test/** — Swipe gesture test app with touch tracking
- **test2/** — Simple test app displaying "Test2" text
- **text_lang_test/** — Multi-language text display test app

### Games (games/)
- **sudoku/** — 6x6 Sudoku puzzle game with number keyboard input and validation
- **minesweeper/** — Classic Minesweeper game with 10x15 grid and 25 mines
- **test/** — Simple test game displaying "Test" text with dashed border

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
  - Calculator with basic arithmetic operations
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
    │   ├── calculator/         — Calculator with arithmetic operations
    │   ├── geometry_test/      — Animated shapes test
    │   ├── reader/             — Text file reader
    │   ├── swipe_test/         — Touch gesture testing
    │   ├── test2/              — Simple test application
    │   └── text_lang_test/     — Multi-language font test
    ├── buttons/                — Button action handlers
    ├── games/                  — Built-in games
    │   ├── minesweeper/        — Classic Minesweeper game
    │   ├── sudoku/             — 6x6 Sudoku puzzle game
    │   └── test/               — Simple test game
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
- **calculator/** — Приложение калькулятора с базовыми арифметическими операциями и функцией AC
- **geometry_test/** — Приложение тестирования геометрии с анимированными фигурами и таймером
- **reader/** — Приложение для чтения текста с списком файлов и пагинацией
- **swipe_test/** — Приложение тестирования жестов свайпа с отслеживанием касаний
- **test2/** — Простое тестовое приложение, отображающее текст "Test2"
- **text_lang_test/** — Приложение тестирования отображения многоязычного текста

### Игры (games/)
- **sudoku/** — Игра-головоломка Судоку 6x6 с вводом чисел через клавиатуру и проверкой правильности
- **minesweeper/** — Классическая игра Сапёр с сеткой 10x15 и 25 минами
- **test/** — Простая тестовая игра, отображающая текст "Test" с пунктирной рамкой

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
- **Калькулятор**: базовые арифметические операции с функцией AC
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
│   ├── calculator/                 # Калькулятор
│   │   ├── app_screen.cpp          # Экран калькулятора с арифметическими операциями
│   │   └── app_screen.h            # Заголовочный файл экрана
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
├── games/                          # Игры
│   ├── minesweeper/                # Сапёр
│   │   ├── game.cpp                # Логика игры Сапёр
│   │   └── game.h                  # Заголовочный файл игры
│   ├── sudoku/                     # Судоку
│   │   ├── game.cpp                # Логика игры Судоку 6x6
│   │   └── game.h                  # Заголовочный файл игры
│   └── test/                       # Тестовая игра
│       ├── game.cpp                # Простая тестовая игра
│       └── game.h                  # Заголовочный файл игры
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

## 日本語版

> **注意：** このプロジェクトはLLM（vibe coding）の支援により作成されました。

> **警告：** これは非常に初期で未完成のバージョンです。電源オフ時でもバッテリーが消耗するという既知の問題を含む多くのバグがあります。

**hi5stack** は、M5Stack/M5Paper（ESP32）ベースのマイクロコントローラーデバイス用プロジェクトで、PlatformIOを使用して実装されています。このプロジェクトは、ファイル管理、SDカード、Wi-Fi、キーボード、および様々な画面をサポートするモジュラーユーザーインターフェースの作成を目的としています。

## アーキテクチャと主要モジュール

### コアモジュール
- **main.cpp** — セットアップ、ループ、タッチ処理を含むアプリケーションのメインエントリーポイント
- **battery.[h/cpp]** — バッテリー電圧と充電率計算機能
- **button.[h/cpp]** — 描画とタッチ処理を含むボタンクラス実装
- **footer.[h/cpp]** — ナビゲーションボタン用フッタークラス実装
- **sdcard.[h/cpp]** — SDカード操作：読み取り、書き込み、存在確認
- **settings.[h/cpp]** — ユーザー設定の保存と管理
- **ui.[h/cpp]** — 基本的なユーザーインターフェース機能
- **sd_gateway.[h/cpp]** — SD Gateway：SDカード上のtxt/jsonファイルのアップロード、削除、一括削除、編集用Webインターフェース
- **debug_config.h** — 様々なシステムコンポーネント用デバッグ設定マクロ

### アプリケーション（apps/）
- **calculator/** — 基本的な算術演算（+、-、×、÷）とAC機能を含む計算機アプリ
- **geometry_test/** — アニメーション図形とタイマーを含む幾何学テストアプリ
- **reader/** — ファイルリストとページネーション機能を含むテキストリーダーアプリ
- **swipe_test/** — タッチトラッキング機能を含むスワイプジェスチャーテストアプリ
- **test2/** — "Test2"テキストを表示するシンプルなテストアプリ
- **text_lang_test/** — 多言語テキスト表示テストアプリ

### ゲーム（games/）
- **sudoku/** — 数字キーボード入力と検証機能付き6x6数独パズルゲーム
- **minesweeper/** — 10x15グリッドと25個の地雷を持つクラシックマインスイーパーゲーム
- **test/** — 破線枠付きで"Test"テキストを表示するシンプルなテストゲーム

### インターフェースコンポーネント
- **buttons/** — 様々なインターフェースボタンの個別ハンドラー（home、files、freeze、off、refresh、rotate）
- **keyboards/** — オンスクリーンキーボードサポート（レイアウト切り替え対応英語キーボード）
- **screens/** — インターフェース画面：メイン、ファイルマネージャー、画像ビューアー、テキストビューアー、Wi-Fi、クリア、電源オフ、アプリ、SD Gateway
- **network/** — スキャンと接続機能を含むWi-Fi接続管理
- **services/** — サービスモジュール（現在は空、将来の拡張用に予約）

## 主な機能

### モジュラーアーキテクチャ
- 画面、ボタン、アプリケーション、サービスの明確な分離
- 新機能追加のための拡張しやすい構造
- システム各コンポーネントの独立モジュール

### ファイルシステムサポート
- 完全なSDカード操作サポート（読み取り、書き込み、削除）
- ディレクトリナビゲーション付きファイルマネージャー
- 様々なファイル形式のサポート（テキスト、画像、JSON）

### 多言語サポート
- 異なる言語でのテキスト表示サポート
- フォントと言語テスト専用アプリ
- 設定可能な言語パラメータ

### 内蔵アプリケーション
- **計算機**：基本的な算術演算（+、-、×、÷）とAC機能
- **テキスト読み取り**：ページネーション機能付きテキストファイル表示
- **幾何学テスト**：タイマー付きアニメーション幾何図形
- **スワイプテスト**：タッチジェスチャーの追跡と可視化
- **フォントテスト**：多言語テキスト表示チェック

### ネットワーク機能
- Wi-Fi接続管理
- 利用可能ネットワークのスキャン
- ファイルへのリモートアクセス用Webインターフェース

### ユーザーインターフェース
- タッチベースナビゲーション
- カスタマイズ可能なボタンとオンスクリーンキーボード
- 様々な画面サイズに対応するアダプティブデザイン

### 画像サポート
- ナビゲーション付き画像表示
- 様々なグラフィック形式のサポート
- 画像のスケーリングと位置調整

### 電源管理
- バッテリーレベルの監視
- 電圧と充電率の計算
- 省電力機能

### SD Gateway
- SDカード上のファイルアクセス用Webインターフェース
- ブラウザ経由でのファイルアップロード、削除、編集
- ファイルの一括操作
- テキストおよびJSONファイルの編集サポート

### デバッグシステム
- 様々なコンポーネント用設定可能デバッグマクロ
- 操作の詳細ログ記録
- 診断とテスト用ツール

## プラットフォーム

- **ハードウェア：** M5Stack/M5Paper（ESP32）
- **開発環境：** PlatformIO

## 始め方

1. PlatformIOをインストールします。
2. PlatformIO IDEでプロジェクトを開きます。
3. M5Stack/M5Paperデバイスを接続します。
4. プロジェクトをビルドしてデバイスにアップロードします。

## リポジトリ構造

```
src/
├── apps/                           # アプリケーション
│   ├── geometry_test/              # アニメーション付き幾何学テスト
│   │   ├── app_screen.cpp          # 幾何図形アプリ画面
│   │   └── app_screen.h            # 画面ヘッダーファイル
│   ├── reader/                     # テキスト読み取りアプリ
│   │   ├── app_screen.cpp          # ページネーション付き読み取り画面
│   │   └── app_screen.h            # 画面ヘッダーファイル
│   ├── swipe_test/                 # スワイプジェスチャーテスト
│   │   ├── app_screen.cpp          # タッチテスト画面
│   │   └── app_screen.h            # 画面ヘッダーファイル
│   ├── test2/                      # シンプルテストアプリ
│   │   ├── app_screen.cpp          # 基本テスト画面
│   │   └── app_screen.h            # 画面ヘッダーファイル
│   └── text_lang_test/             # 多言語テキストテスト
│       ├── app_screen.cpp          # フォントテスト画面
│       └── app_screen.h            # 画面ヘッダーファイル
├── buttons/                        # インターフェースボタンハンドラー
│   ├── files_button.cpp            # ファイルマネージャーボタン
│   ├── freeze_button.cpp           # 画面フリーズボタン
│   ├── home_button.cpp             # ホーム画面戻りボタン
│   ├── off_button.cpp              # 電源オフボタン
│   ├── refresh_button.cpp          # 更新ボタン
│   └── rotate_button.cpp           # 画面回転ボタン
├── keyboards/                      # オンスクリーンキーボード
│   ├── en_keyboard.cpp             # 英語キーボード
│   └── en_keyboard.h               # キーボードヘッダーファイル
├── network/                        # ネットワーク機能
│   ├── wifi_manager.cpp            # Wi-Fi接続管理
│   └── wifi_manager.h              # Wi-Fiマネージャーヘッダーファイル
├── screens/                        # ユーザーインターフェース画面
│   ├── apps_screen.cpp             # アプリリスト画面
│   ├── clear_screen.cpp            # クリア画面
│   ├── files_screen.cpp            # ファイルマネージャー画面
│   ├── image_screen.cpp            # 画像表示画面
│   ├── main_screen.cpp             # メイン画面
│   ├── off_screen.cpp              # 電源オフ画面
│   ├── sd_gateway_screen.cpp       # SD Gateway画面
│   ├── text_screen.cpp             # テキスト表示画面
│   └── wifi_screen.cpp             # Wi-Fi設定画面
├── services/                       # サービスモジュール（予約済み）
├── battery.cpp                     # バッテリー監視機能
├── battery.h                       # バッテリーヘッダーファイル
├── button.cpp                      # ボタンクラス実装
├── button.h                        # ボタンヘッダーファイル
├── debug_config.h                  # デバッグ設定
├── footer.cpp                      # フッター実装
├── footer.h                        # フッターヘッダーファイル
├── main.cpp                        # アプリケーションメインエントリーポイント
├── sd_gateway.cpp                  # SD Gateway実装
├── sd_gateway.h                    # SD Gatewayヘッダーファイル
├── sdcard.cpp                      # SDカード操作
├── sdcard.h                        # SDカードヘッダーファイル
├── settings.cpp                    # 設定管理
├── settings.h                      # 設定ヘッダーファイル
├── ui.cpp                          # 基本インターフェース機能
└── ui.h                            # インターフェースヘッダーファイル
```

---

プロジェクトは新しい画面、サービス、ボタンハンドラーの追加により簡単に拡張できます。

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
- **calculator/** — 计算器应用，包含基本算术运算（+、-、×、÷）和AC功能
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
- **计算器**：基本算术运算（+、-、×、÷）和AC功能
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

---

## हिंदी अनुवाद

> **नोट:** यह प्रोजेक्ट LLM (vibe coding) की सहायता से बनाया गया है।

> **चेतावनी:** यह एक बहुत ही प्रारंभिक और कच्चा संस्करण है। इसमें कई बग हैं, जिसमें एक ज्ञात समस्या भी शामिल है जहाँ डिवाइस बंद होने पर भी बैटरी खत्म हो जाती है।

**hi5stack** M5Stack/M5Paper (ESP32) आधारित माइक्रोकंट्रोलर डिवाइसों के लिए एक प्रोजेक्ट है, जो PlatformIO का उपयोग करके लागू किया गया है। यह प्रोजेक्ट फाइल प्रबंधन, SD कार्ड, Wi-Fi, कीबोर्ड और विभिन्न स्क्रीन के समर्थन के साथ एक मॉड्यूलर यूजर इंटरफेस बनाने के लिए डिज़ाइन किया गया है।

## आर्किटेक्चर और मुख्य मॉड्यूल

### कोर मॉड्यूल
- **main.cpp** — सेटअप, लूप और टच हैंडलिंग के साथ एप्लिकेशन का मुख्य एंट्री पॉइंट
- **battery.[h/cpp]** — बैटरी वोल्टेज और प्रतिशत गणना फ़ंक्शन
- **button.[h/cpp]** — ड्राइंग और टच हैंडलिंग के साथ बटन क्लास इम्प्लीमेंटेशन
- **footer.[h/cpp]** — नेवीगेशन बटन के लिए फुटर क्लास इम्प्लीमेंटेशन
- **sdcard.[h/cpp]** — SD कार्ड ऑपरेशन: रीडिंग, राइटिंग, उपस्थिति जांच
- **settings.[h/cpp]** — यूजर सेटिंग्स का भंडारण और प्रबंधन
- **ui.[h/cpp]** — बुनियादी यूजर इंटरफेस फ़ंक्शन
- **sd_gateway.[h/cpp]** — SD Gateway: SD कार्ड पर txt/json फाइलों को अपलोड, डिलीट, बल्क डिलीट और एडिट करने के लिए वेब इंटरफेस
- **debug_config.h** — विभिन्न सिस्टम कंपोनेंट्स के लिए डिबग कॉन्फ़िगरेशन मैक्रो

### एप्लिकेशन (apps/)
- **calculator/** — बुनियादी अंकगणितीय संचालन (+, -, ×, ÷) और AC फ़ंक्शन के साथ कैलकुलेटर ऐप
- **geometry_test/** — एनिमेटेड आकार और टाइमर के साथ ज्यामिति टेस्ट ऐप
- **reader/** — फाइल लिस्ट और पेजिनेशन के साथ टेक्स्ट रीडर ऐप
- **swipe_test/** — टच ट्रैकिंग के साथ स्वाइप जेस्चर टेस्ट ऐप
- **test2/** — "Test2" टेक्स्ट दिखाने वाला सरल टेस्ट ऐप
- **text_lang_test/** — बहुभाषी टेक्स्ट डिस्प्ले टेस्ट ऐप

### इंटरफेस कंपोनेंट्स
- **buttons/** — विभिन्न इंटरफेस बटन के लिए अलग हैंडलर (home, files, freeze, off, refresh, rotate)
- **keyboards/** — ऑन-स्क्रीन कीबोर्ड सपोर्ट (लेआउट स्विचिंग के साथ अंग्रेजी कीबोर्ड)
- **screens/** — इंटरफेस स्क्रीन: मुख्य, फाइल मैनेजर, इमेज व्यूअर, टेक्स्ट व्यूअर, Wi-Fi, क्लियर, पावर ऑफ, ऐप्स, SD Gateway
- **network/** — स्कैनिंग और कनेक्शन फीचर्स के साथ Wi-Fi कनेक्शन प्रबंधन
- **services/** — सर्विस मॉड्यूल (वर्तमान में खाली, भविष्य के विस्तार के लिए आरक्षित)

## मुख्य विशेषताएं

### मॉड्यूलर आर्किटेक्चर
- स्क्रीन, बटन, एप्लिकेशन और सेवाओं का स्पष्ट विभाजन
- नई सुविधाओं को जोड़ने के लिए आसानी से विस्तार योग्य संरचना
- सिस्टम के विभिन्न कंपोनेंट्स के लिए स्वतंत्र मॉड्यूल

### फाइल सिस्टम सपोर्ट
- पूर्ण SD कार्ड ऑपरेशन सपोर्ट (रीड, राइट, डिलीट)
- डायरेक्टरी नेवीगेशन के साथ फाइल मैनेजर
- विभिन्न फाइल फॉर्मेट का समर्थन (टेक्स्ट, इमेज, JSON)

### बहुभाषी समर्थन
- विभिन्न भाषाओं में टेक्स्ट डिस्प्ले सपोर्ट
- फॉन्ट और भाषा टेस्टिंग के लिए विशेष ऐप
- कॉन्फ़िगरेबल भाषा पैरामीटर

### अंतर्निहित एप्लिकेशन
- **कैलकुलेटर**: बुनियादी अंकगणितीय संचालन (+, -, ×, ÷) और AC फ़ंक्शन
- **टेक्स्ट रीडिंग**: पेजिनेशन के साथ टेक्स्ट फाइल व्यूइंग
- **ज्यामिति टेस्ट**: टाइमर के साथ एनिमेटेड ज्यामितीय आकार
- **स्वाइप टेस्ट**: टच जेस्चर ट्रैकिंग और विज़ुअलाइज़ेशन
- **फॉन्ट टेस्ट**: बहुभाषी टेक्स्ट डिस्प्ले चेक

### नेटवर्क क्षमताएं
- Wi-Fi कनेक्शन प्रबंधन
- उपलब्ध नेटवर्क स्कैनिंग
- फाइलों तक रिमोट एक्सेस के लिए वेब इंटरफेस

### यूजर इंटरफेस
- टच-आधारित नेवीगेशन
- कस्टमाइज़ेबल बटन और ऑन-स्क्रीन कीबोर्ड
- विभिन्न स्क्रीन साइज़ के लिए एडाप्टिव डिज़ाइन

### इमेज सपोर्ट
- नेवीगेशन के साथ इमेज व्यूइंग
- विभिन्न ग्राफिक फॉर्मेट का समर्थन
- इमेज स्केलिंग और पोज़िशनिंग

### पावर मैनेजमेंट
- बैटरी लेवल मॉनिटरिंग
- वोल्टेज और चार्ज प्रतिशत की गणना
- पावर सेविंग फीचर्स

### SD Gateway
- SD कार्ड पर फाइलों तक पहुंच के लिए वेब इंटरफेस
- ब्राउज़र के माध्यम से फाइल अपलोड, डिलीट और एडिट
- फाइलों के बल्क ऑपरेशन
- टेक्स्ट और JSON फाइलों की एडिटिंग सपोर्ट

### डिबग सिस्टम
- विभिन्न कंपोनेंट्स के लिए कॉन्फ़िगरेबल डिबग मैक्रो
- ऑपरेशन की विस्तृत लॉगिंग
- डायग्नोसिस और टेस्टिंग के लिए टूल्स

## प्लेटफॉर्म

- **हार्डवेयर:** M5Stack/M5Paper (ESP32)
- **डेवलपमेंट एनवायरनमेंट:** PlatformIO

## शुरुआत करना

1. PlatformIO इंस्टॉल करें।
2. PlatformIO IDE में प्रोजेक्ट खोलें।
3. अपना M5Stack/M5Paper डिवाइस कनेक्ट करें।
4. प्रोजेक्ट को बिल्ड करें और डिवाइस पर अपलोड करें।

## रिपॉज़िटरी संरचना

```
src/
├── apps/                           # एप्लिकेशन
│   ├── geometry_test/              # एनिमेशन के साथ ज्यामिति टेस्ट
│   │   ├── app_screen.cpp          # ज्यामितीय आकार ऐप स्क्रीन
│   │   └── app_screen.h            # स्क्रीन हेडर फाइल
│   ├── reader/                     # टेक्स्ट रीडिंग ऐप
│   │   ├── app_screen.cpp          # पेजिनेशन के साथ रीडिंग स्क्रीन
│   │   └── app_screen.h            # स्क्रीन हेडर फाइल
│   ├── swipe_test/                 # स्वाइप जेस्चर टेस्ट
│   │   ├── app_screen.cpp          # टच टेस्ट स्क्रीन
│   │   └── app_screen.h            # स्क्रीन हेडर फाइल
│   ├── test2/                      # सरल टेस्ट ऐप
│   │   ├── app_screen.cpp          # बेसिक टेस्ट स्क्रीन
│   │   └── app_screen.h            # स्क्रीन हेडर फाइल
│   └── text_lang_test/             # बहुभाषी टेक्स्ट टेस्ट
│       ├── app_screen.cpp          # फॉन्ट टेस्ट स्क्रीन
│       └── app_screen.h            # स्क्रीन हेडर फाइल
├── buttons/                        # इंटरफेस बटन हैंडलर
│   ├── files_button.cpp            # फाइल मैनेजर बटन
│   ├── freeze_button.cpp           # स्क्रीन फ्रीज़ बटन
│   ├── home_button.cpp             # होम स्क्रीन रिटर्न बटन
│   ├── off_button.cpp              # पावर ऑफ बटन
│   ├── refresh_button.cpp          # रिफ्रेश बटन
│   └── rotate_button.cpp           # स्क्रीन रोटेट बटन
├── keyboards/                      # ऑन-स्क्रीन कीबोर्ड
│   ├── en_keyboard.cpp             # अंग्रेजी कीबोर्ड
│   └── en_keyboard.h               # कीबोर्ड हेडर फाइल
├── network/                        # नेटवर्क फ़ंक्शन
│   ├── wifi_manager.cpp            # Wi-Fi कनेक्शन प्रबंधन
│   └── wifi_manager.h              # Wi-Fi मैनेजर हेडर फाइल
├── screens/                        # यूजर इंटरफेस स्क्रीन
│   ├── apps_screen.cpp             # ऐप लिस्ट स्क्रीन
│   ├── clear_screen.cpp            # क्लियर स्क्रीन
│   ├── files_screen.cpp            # फाइल मैनेजर स्क्रीन
│   ├── image_screen.cpp            # इमेज डिस्प्ले स्क्रीन
│   ├── main_screen.cpp             # मुख्य स्क्रीन
│   ├── off_screen.cpp              # पावर ऑफ स्क्रीन
│   ├── sd_gateway_screen.cpp       # SD Gateway स्क्रीन
│   ├── text_screen.cpp             # टेक्स्ट डिस्प्ले स्क्रीन
│   └── wifi_screen.cpp             # Wi-Fi सेटिंग स्क्रीन
├── services/                       # सर्विस मॉड्यूल (आरक्षित)
├── battery.cpp                     # बैटरी मॉनिटरिंग फ़ंक्शन
├── battery.h                       # बैटरी हेडर फाइल
├── button.cpp                      # बटन क्लास इम्प्लीमेंटेशन
├── button.h                        # बटन हेडर फाइल
├── debug_config.h                  # डिबग कॉन्फ़िगरेशन
├── footer.cpp                      # फुटर इम्प्लीमेंटेशन
├── footer.h                        # फुटर हेडर फाइल
├── main.cpp                        # एप्लिकेशन मुख्य एंट्री पॉइंट
├── sd_gateway.cpp                  # SD Gateway इम्प्लीमेंटेशन
├── sd_gateway.h                    # SD Gateway हेडर फाइल
├── sdcard.cpp                      # SD कार्ड ऑपरेशन
├── sdcard.h                        # SD कार्ड हेडर फाइल
├── settings.cpp                    # सेटिंग प्रबंधन
├── settings.h                      # सेटिंग हेडर फाइल
├── ui.cpp                          # बुनियादी इंटरफेस फ़ंक्शन
└── ui.h                            # इंटरफेस हेडर फाइल
```

---

प्रोजेक्ट को नई स्क्रीन, सेवाओं और बटन हैंडलर जोड़कर आसानी से विस्तारित किया जा सकता है।

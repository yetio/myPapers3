# Project Tree

M5Stack Papers3 project file tree:

```
├── .gitignore
├── .vscode/
│   ├── extensions.json
│   └── settings.json
├── README.md
├── platformio.ini
├── project_tree.md
└── src/
    ├── apps/
    │   ├── geometry_test/
    │   │   ├── app_screen.cpp - Geometry test app with animated shapes and timer
    │   │   └── app_screen.h - Header file for geometry test app screen functions
    │   ├── reader/
    │   │   ├── app_screen.cpp - Text reader app with file list and pagination
    │   │   └── app_screen.h - Header file for text reader app functions
    │   ├── swipe_test/
    │   │   ├── app_screen.cpp - Swipe gesture test app with touch tracking
    │   │   └── app_screen.h - Header file for swipe test app functions
    │   ├── test2/
    │   │   ├── app_screen.cpp - Simple test app displaying "Test2" text
    │   │   └── app_screen.h - Header file for test2 app functions
    │   └── text_lang_test/
    │       ├── app_screen.cpp - Multi-language text display test app
    │       └── app_screen.h - Header file for text language test app functions
    ├── battery.cpp - Battery voltage and percentage calculation functions
    ├── battery.h - Header file for battery management functions
    ├── button.cpp - Button class implementation with drawing and touch handling
    ├── button.h - Header file for Button class definition
    ├── buttons/
    │   ├── files.cpp - Files button action implementation
    │   ├── files.h - Header file for files button functions
    │   ├── freeze.cpp - Freeze button action with power off functionality
    │   ├── freeze.h - Header file for freeze button functions
    │   ├── home.cpp - Home button action implementation
    │   ├── home.h - Header file for home button functions
    │   ├── off.cpp - Off button action with deep sleep
    │   ├── off.h - Header file for off button functions
    │   ├── rfrsh.cpp - Refresh button action implementation
    │   ├── rfrsh.h - Header file for refresh button functions
    │   ├── rotate.cpp - Rotation button actions for images and text
    │   └── rotate.h - Header file for rotation button functions
    ├── debug_config.h - Debug configuration macros for various system components
    ├── footer.cpp - Footer class implementation for bottom navigation buttons
    ├── footer.h - Header file for Footer class and FooterButton structure
    ├── keyboards/
    │   ├── eng_keyboard.cpp - English keyboard implementation with layout switching
    │   └── eng_keyboard.h - Header file for English keyboard functions and layouts
    ├── main.cpp - Main application entry point with setup, loop, and touch handling
    ├── network/
    │   ├── wifi_manager.cpp - WiFi manager implementation with scanning and connection
    │   └── wifi_manager.h - Header file for WiFi manager singleton class
    ├── screens/
    │   ├── apps_screen.cpp - Applications screen implementation with app selection
    │   ├── apps_screen.h - Header file for applications screen functions
    │   ├── clear_screen.cpp - Screen clearing functionality implementation
    │   ├── clear_screen.h - Header file for screen clearing functions
    │   ├── files_screen.cpp - File manager screen implementation with pagination
    │   ├── files_screen.h - Header file for file manager screen functions
    │   ├── img_viewer_screen.cpp - Image viewer screen implementation with BMP support
    │   ├── img_viewer_screen.h - Header file for image viewer screen functions
    │   ├── main_screen.cpp - Main screen implementation with system status display
    │   ├── main_screen.h - Header file for main screen functions
    │   ├── off_screen.cpp - Power off screen implementation with device shutdown
    │   ├── off_screen.h - Header file for power off screen functions
    │   ├── sd_gateway_screen.cpp - SD Gateway screen implementation with web interface status
    │   ├── sd_gateway_screen.h - Header file for SD Gateway screen functions
    │   ├── txt_viewer_screen.cpp - Text viewer screen implementation with word wrapping
    │   ├── txt_viewer_screen.h - Header file for text viewer screen functions
    │   ├── wifi_screen.cpp - WiFi screen implementation with network scanning and connection
    │   └── wifi_screen.h - Header file for WiFi screen functions
    ├── sd_gateway.cpp
    ├── sd_gateway.h
    ├── sdcard.cpp
    ├── sdcard.h
    ├── services/
    ├── settings.cpp
    ├── settings.h
    ├── ui.cpp
    └── ui.h
```

## Structure Description

### Root Files
- `.gitignore` - Git exclusion file
- `README.md` - project documentation
- `build_log.txt` - build log
- `platformio.ini` - PlatformIO configuration
- `project_tree.md` - this file with project tree

### Directories
- `.vscode/` - Visual Studio Code settings
- `data/` - project data
- `src/` - source code

### Source Code (src/)
- `apps/` - applications (geometry_test, reader, swipe_test, test2, text_lang_test)
- `buttons/` - button handlers
- `keyboards/` - keyboards
- `network/` - network functions
- `screens/` - interface screens
- `services/` - services
- Core modules: battery, button, footer, main, sd_gateway, sdcard, settings, ui
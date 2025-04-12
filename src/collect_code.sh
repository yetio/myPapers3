#!/bin/bash

# Script name
SCRIPT_NAME=$(basename "$0")

# Output file
OUTPUT_FILE="code.txt"

# Separator line
SEPARATOR="----------------------------------------"

# Clear (or create) output file
> "$OUTPUT_FILE"

# Insert two empty lines and the required text
echo -e "\n\n" >> "$OUTPUT_FILE"
echo "пиши объяснения по минимуму на русском. Показывай только те файлы (целиком), которые надо исправить. Если файлы менять не надо, то не показывай их и не пиши, что там что-то надо проверить. Не повторяй показ файлов несколько раз, достаточно одного, финального показа кода файла. Код должен быть модульным и с минимальной зависимостью. Пиши названия файлов вместе с их путём. В ответе файл больше одного раза показывать не надо. Комментарии в коде пиши на английском. platformio.ini
[env:esp32-s3-devkitm-1]
platform = espressif32
board = esp32-s3-devkitm-1
framework = arduino
monitor_speed = 115200
upload_speed = 115200
board_upload.flash_size = 16MB
board_upload.maximum_size = 16777216
board_build.arduino.memory_type = qio_opi
build_flags =
    -DBOARD_HAS_PSRAM
    -DLGFX_USE_V1
    -std=c++17
lib_deps =
    epdiy=https://github.com/vroland/epdiy.git
    m5stack/M5GFX @ ^0.2.3
    m5stack/M5Unified @ ^0.2.2" >> "$OUTPUT_FILE"
echo -e "\n" >> "$OUTPUT_FILE"

# Build file list
echo "Список файлов проекта:" >> "$OUTPUT_FILE"
echo "$SEPARATOR" >> "$OUTPUT_FILE"

# Process all files (excluding hidden, system, script itself and output file)
find . -type f ! -name "$SCRIPT_NAME" ! -name "$OUTPUT_FILE" ! -path "*/.*" | sort >> "$OUTPUT_FILE"

echo -e "\n$SEPARATOR\n" >> "$OUTPUT_FILE"

# Add file contents to the end
find . -type f ! -name "$SCRIPT_NAME" ! -name "$OUTPUT_FILE" ! -path "*/.*" | while read -r file; do
    echo "Файл: $file" >> "$OUTPUT_FILE"
    echo "$SEPARATOR" >> "$OUTPUT_FILE"
    cat "$file" >> "$OUTPUT_FILE"
    echo -e "\n$SEPARATOR\n" >> "$OUTPUT_FILE"
done

echo "Сбор кода завершён. Содержимое сохранено в $OUTPUT_FILE."
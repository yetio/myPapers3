// ./screens/img_viewer_screen.cpp
#include "img_viewer_screen.h"
#include "../ui.h"
#include "../sdcard.h"

// Перечисление для поддерживаемых форматов изображений
enum ImageFormat {
    FORMAT_UNKNOWN,
    FORMAT_BMP,
    FORMAT_JPG,
    FORMAT_PNG
};



// Функция для определения формата изображения по расширению файла
ImageFormat getImageFormat(const String& filename) {
    String ext = filename.substring(filename.lastIndexOf('.') + 1);
    ext.toLowerCase();
    
    if (ext == "bmp") return FORMAT_BMP;
    if (ext == "jpg" || ext == "jpeg") return FORMAT_JPG;
    if (ext == "png") return FORMAT_PNG;
    
    return FORMAT_UNKNOWN;
}

// Функция для получения размеров JPG изображения из заголовка файла
bool getJpgDimensions(const String& filename, int& width, int& height) {
    File file = SD.open(filename, FILE_READ);
    if (!file) {
        return false;
    }
    
    // JPEG файлы начинаются с маркера SOI (Start of Image) - 0xFFD8
    uint8_t marker[2];
    if (file.read(marker, 2) != 2 || marker[0] != 0xFF || marker[1] != 0xD8) {
        file.close();
        return false;
    }
    
    // Ищем маркер SOF (Start of Frame), который содержит размеры
    // SOF маркеры: 0xFFC0-0xFFC3, 0xFFC5-0xFFC7, 0xFFC9-0xFFCB, 0xFFCD-0xFFCF
    while (!file.available()) {
        if (file.read(marker, 2) != 2 || marker[0] != 0xFF) {
            file.close();
            return false;
        }
        
        // Проверяем, является ли маркер одним из SOF маркеров
        if ((marker[1] >= 0xC0 && marker[1] <= 0xC3) || 
            (marker[1] >= 0xC5 && marker[1] <= 0xC7) || 
            (marker[1] >= 0xC9 && marker[1] <= 0xCB) || 
            (marker[1] >= 0xCD && marker[1] <= 0xCF)) {
            
            // Пропускаем длину сегмента (2 байта)
            file.read(marker, 2);
            
            // Пропускаем точность (1 байт)
            file.read();
            
            // Читаем высоту и ширину (по 2 байта каждый, в формате big-endian)
            uint8_t dimensions[4];
            if (file.read(dimensions, 4) != 4) {
                file.close();
                return false;
            }
            
            // Высота и ширина в JPEG хранятся в формате big-endian
            height = (dimensions[0] << 8) | dimensions[1];
            width = (dimensions[2] << 8) | dimensions[3];
            
            file.close();
            return true;
        }
        
        // Если это не SOF маркер, пропускаем сегмент
        uint16_t segmentLength;
        if (file.read((uint8_t*)&segmentLength, 2) != 2) {
            file.close();
            return false;
        }
        
        // Преобразуем из big-endian и вычитаем 2 (длина уже включает эти 2 байта)
        segmentLength = ((segmentLength & 0xFF) << 8) | ((segmentLength >> 8) & 0xFF);
        segmentLength -= 2;
        
        // Пропускаем данные сегмента
        file.seek(file.position() + segmentLength);
    }
    
    file.close();
    return false;
}

// Функция для отображения JPG изображений
bool displayJpgImage(const String& filename, int centerX, int centerY, int maxWidth, int maxHeight) {
    Serial.println("[DEBUG] displayJpgImage called");
    Serial.println("[DEBUG] Filename: " + filename);
    Serial.printf("[DEBUG] Center: (%d, %d), Size: %dx%d\n", centerX, centerY, maxWidth, maxHeight);
    
    // Проверяем существование файла
    if (!SD.exists(filename)) {
        Serial.println("[ERROR] JPG file does not exist: " + filename);
        return false;
    }
    
    // Рассчитываем позицию для центрирования (как для PNG)
    int posX = centerX - (maxWidth / 2);
    int posY = centerY - (maxHeight / 2);
    Serial.printf("[DEBUG] Calculated position: (%d, %d)\n", posX, posY);
    
    // Используем простой вызов M5.Lcd.drawJpgFile (как для PNG)
    Serial.println("[DEBUG] Calling M5.Lcd.drawJpgFile...");
    M5.Lcd.drawJpgFile(filename.c_str(), posX, posY);
    Serial.println("[DEBUG] M5.Lcd.drawJpgFile completed");
    return true;
}

// Функция для получения размеров PNG изображения из заголовка файла
bool getPngDimensions(const String& filename, int& width, int& height) {
    File file = SD.open(filename, FILE_READ);
    if (!file) {
        return false;
    }
    
    // PNG файлы начинаются с сигнатуры из 8 байт
    uint8_t signature[8];
    if (file.read(signature, 8) != 8) {
        file.close();
        return false;
    }
    
    // Проверяем сигнатуру PNG
    static const uint8_t PNG_SIGNATURE[8] = {137, 80, 78, 71, 13, 10, 26, 10};
    for (int i = 0; i < 8; i++) {
        if (signature[i] != PNG_SIGNATURE[i]) {
            file.close();
            return false;
        }
    }
    
    // После сигнатуры идет чанк IHDR, который содержит размеры
    // Формат: длина (4 байта) + тип (4 байта) + данные + CRC (4 байта)
    uint8_t chunkHeader[8];
    if (file.read(chunkHeader, 8) != 8) {
        file.close();
        return false;
    }
    
    // Проверяем, что это чанк IHDR (тип "IHDR")
    if (chunkHeader[4] != 'I' || chunkHeader[5] != 'H' || chunkHeader[6] != 'D' || chunkHeader[7] != 'R') {
        file.close();
        return false;
    }
    
    // Читаем ширину и высоту (по 4 байта каждый, в формате big-endian)
    uint8_t dimensions[8];
    if (file.read(dimensions, 8) != 8) {
        file.close();
        return false;
    }
    
    // Ширина и высота в PNG хранятся в формате big-endian
    width = (dimensions[0] << 24) | (dimensions[1] << 16) | (dimensions[2] << 8) | dimensions[3];
    height = (dimensions[4] << 24) | (dimensions[5] << 16) | (dimensions[6] << 8) | dimensions[7];
    
    file.close();
    return true;
}

// Функция для отображения PNG изображений
bool displayPngImage(const String& filename, int centerX, int centerY, int maxWidth, int maxHeight) {
    Serial.println("[DEBUG] displayPngImage called");
    Serial.println("[DEBUG] Filename: " + filename);
    Serial.printf("[DEBUG] Center: (%d, %d), Size: %dx%d\n", centerX, centerY, maxWidth, maxHeight);
    
    // Проверяем существование файла
    if (!SD.exists(filename)) {
        Serial.println("[ERROR] PNG file does not exist: " + filename);
        return false;
    }
    
    // Рассчитываем позицию для центрирования
    int posX = centerX - (maxWidth / 2);
    int posY = centerY - (maxHeight / 2);
    Serial.printf("[DEBUG] Calculated position: (%d, %d)\n", posX, posY);
    
    // Используем M5.Lcd.drawPngFile
    Serial.println("[DEBUG] Calling M5.Lcd.drawPngFile...");
    M5.Lcd.drawPngFile(filename.c_str(), posX, posY, maxWidth, maxHeight);
    Serial.println("[DEBUG] M5.Lcd.drawPngFile completed");
    return true;
}



namespace screens {
    static String currentImgOpened = "";

    // Frame corner coordinates
    static constexpr int frameLeft = 0;      // Left X-coordinate
    static constexpr int frameTop = 100;     // Top Y-coordinate
    static constexpr int frameRight = 540;   // Right X-coordinate
    static constexpr int frameBottom = 700;  // Bottom Y-coordinate

    void drawImgViewerScreen(const String& filename) {
        currentImgOpened = filename;

        // Clear the screen
        M5.Display.fillScreen(TFT_WHITE);

        // Draw frame for visual reference
        M5.Display.drawRect(frameLeft, frameTop, frameRight - frameLeft, frameBottom - frameTop, TFT_BLACK);

        // Display the image in the designated frame
        displayImgFile(filename);

        // Setup footer buttons for viewer screens
        FooterButton viewerFooterButtons[] = {
            {"Home", homeAction},
            {"Off", showOffScreen},
            {"Freeze", freezeAction},
            {"Files", filesAction}
        };
        footer.setButtons(viewerFooterButtons, 4);

        // Render all buffered rows
        ::drawRowsBuffered();
        // Draw footer
        footer.draw(footer.isVisible());
        M5.Display.display();
    }

    bool getBmpDimensions(const uint8_t* data, size_t size, int& width, int& height) {
        if (size < 26) return false;
        // BMP width is at offset 18, height at offset 22
        width = data[18] | (data[19] << 8) | (data[20] << 16) | (data[21] << 24);
        height = data[22] | (data[23] << 8) | (data[24] << 16) | (data[25] << 24);
        return true;
    }

    // Nearest-neighbor scaling for BMP images
    void scaleBmpImage(const uint8_t* srcData, int srcWidth, int srcHeight, uint16_t* destBuffer, int destWidth, int destHeight) {
        // Calculate row size with padding (each row is padded to a multiple of 4 bytes)
        int rowSize = ((24 * srcWidth + 31) / 32) * 4;

        for(int y = 0; y < destHeight; y++) {
            for(int x = 0; x < destWidth; x++) {
                int srcX = x * srcWidth / destWidth;
                int srcY = srcHeight - 1 - (y * srcHeight / destHeight); // Invert Y for BMP

                // Clamp source coordinates
                srcX = constrain(srcX, 0, srcWidth - 1);
                srcY = constrain(srcY, 0, srcHeight - 1);

                int srcIndex = (srcY * rowSize) + (srcX * 3); // 24-bit BMP

                uint8_t blue = srcData[srcIndex];
                uint8_t green = srcData[srcIndex + 1];
                uint8_t red = srcData[srcIndex + 2];
                destBuffer[y * destWidth + x] = M5.Display.color565(red, green, blue);
            }
        }
    }

    void displayImgFile(const String& filename) {
        Serial.println("[DEBUG] displayImgFile called with: " + filename);
        
        ImageFormat format = getImageFormat(filename);
        Serial.printf("[DEBUG] Detected format: %d\n", (int)format);
        
        if (format == FORMAT_UNKNOWN) {
            Serial.println("[ERROR] Unknown image format");
            ::setUniversalFont();
            M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
            M5.Display.drawString("Unsupported file format", frameLeft + 20, frameTop + 20);
            return;
        }
        
        // Open file
        File file = SD.open(filename, FILE_READ);
        if (!file) {
            ::setUniversalFont();
            M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
            M5.Display.drawString("Error opening file", frameLeft + 20, frameTop + 20);
            return;
        }
        
        // Check file size
        size_t fileSize = file.size();
        if (fileSize == 0 || fileSize > 8 * 1024 * 1024) {
            ::setUniversalFont();
            M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
            M5.Display.drawString("Invalid file size", frameLeft + 20, frameTop + 20);
            file.close();
            return;
        }
        
        // Calculate position to center the image in frame
        int frameWidth = frameRight - frameLeft;
        int frameHeight = frameBottom - frameTop;
        int posX = frameLeft + (frameWidth / 2);
        int posY = frameTop + (frameHeight / 2);
        
        // Set clipping rectangle to frame boundaries
        M5.Display.setClipRect(frameLeft, frameTop, frameWidth, frameHeight);
        
        bool success = false;
        
        switch (format) {
            case FORMAT_BMP:
                // Существующий код для BMP
                {
                    uint8_t *fileData = (uint8_t *)malloc(fileSize);
                    if (!fileData) {
                        ::setUniversalFont();
                        M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
                        M5.Display.drawString("Out of memory", frameLeft + 20, frameTop + 20);
                        file.close();
                        return;
                    }
                    
                    size_t bytesRead = file.read(fileData, fileSize);
                    if (bytesRead != fileSize) {
                        ::setUniversalFont();
                        M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
                        M5.Display.drawString("Error reading file", frameLeft + 20, frameTop + 20);
                        free(fileData);
                        file.close();
                        return;
                    }
                    
                    int imgWidth = 0, imgHeight = 0;
                    bool dimensionSuccess = getBmpDimensions(fileData, fileSize, imgWidth, imgHeight);
                    
                    if (dimensionSuccess) {
                        // Calculate scaling factor to fit within frame
                        float scale = min((float)frameWidth / imgWidth, (float)frameHeight / imgHeight);
                        int scaledWidth = floor(imgWidth * scale);
                        int scaledHeight = floor(imgHeight * scale);
                        
                        // Ensure scaled dimensions fit within frame
                        scaledWidth = min(scaledWidth, frameWidth);
                        scaledHeight = min(scaledHeight, frameHeight);
                        
                        // Calculate position to center the image
                        posX = frameLeft + (frameWidth - scaledWidth) / 2;
                        posY = frameTop + (frameHeight - scaledHeight) / 2;
                        
                        // Extract pixel data (24-bit BMP with 54-byte header)
                        const uint8_t* pixelData = fileData + 54;
                        
                        // Allocate buffer for scaled image
                        size_t bufferSize = scaledWidth * scaledHeight * sizeof(uint16_t);
                        uint16_t* scaledBuffer = (uint16_t*)malloc(bufferSize);
                        if (!scaledBuffer) {
                            ::setUniversalFont();
                            M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
                            M5.Display.drawString("Out of memory for scaling", frameLeft + 20, frameTop + 20);
                            free(fileData);
                            file.close();
                            return;
                        }
                        
                        scaleBmpImage(pixelData, imgWidth, imgHeight, scaledBuffer, scaledWidth, scaledHeight);
                        // Draw scaled image
                        M5.Display.pushImage(posX, posY, scaledWidth, scaledHeight, scaledBuffer);
                        free(scaledBuffer);
                        success = true;
                    }
                    
                    free(fileData);
                }
                break;
                
            case FORMAT_JPG:
                Serial.println("[DEBUG] Processing JPG format");
                Serial.printf("[DEBUG] Frame dimensions: %dx%d, position: (%d, %d)\n", frameWidth, frameHeight, posX, posY);
                // Используем встроенную функцию M5GFX для JPG
                success = displayJpgImage(filename, posX, posY, frameWidth, frameHeight);
                Serial.printf("[DEBUG] JPG display result: %s\n", success ? "SUCCESS" : "FAILED");
                break;
                
            case FORMAT_PNG:
                Serial.println("[DEBUG] Processing PNG format");
                Serial.printf("[DEBUG] Frame dimensions: %dx%d, position: (%d, %d)\n", frameWidth, frameHeight, posX, posY);
                // Используем встроенную функцию M5GFX для PNG
                success = displayPngImage(filename, posX, posY, frameWidth, frameHeight);
                Serial.printf("[DEBUG] PNG display result: %s\n", success ? "SUCCESS" : "FAILED");
                break;
                
            default:
                success = false;
                break;
        }
        
        file.close();
        
        // Reset clipping rectangle
        M5.Display.setClipRect(0, 0, EPD_WIDTH, EPD_HEIGHT);
        
        if (!success) {
            ::setUniversalFont();
            M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
            M5.Display.drawString("Error displaying image", frameLeft + 20, frameTop + 20);
        }
    }

    // Функция displayFullScreenImgFile перемещена ниже и поддерживает все форматы изображений

    void clearImgViewerScreen() {
        // Clear the entire screen
        M5.Display.fillScreen(TFT_WHITE);
        M5.Display.display();
    }

    String getCurrentImgFile() {
        return screens::currentImgOpened;
    }
    
    void displayFullScreenImgFile(const String& filename) {
        ImageFormat format = getImageFormat(filename);
        
        if (format == FORMAT_UNKNOWN) {
            ::setUniversalFont();
            M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
            M5.Display.drawString("Unsupported file format", 20, 20);
            return;
        }
        
        // Open file
        File file = SD.open(filename, FILE_READ);
        if (!file) {
            ::setUniversalFont();
            M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
            M5.Display.drawString("Error opening file", 20, 20);
            return;
        }
        
        // Check file size
        size_t fileSize = file.size();
        if (fileSize == 0 || fileSize > 8 * 1024 * 1024) {
            ::setUniversalFont();
            M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
            M5.Display.drawString("Invalid file size", 20, 20);
            file.close();
            return;
        }
        
        // Для полноэкранного режима используем весь экран
        int screenWidth = EPD_WIDTH;
        int screenHeight = EPD_HEIGHT;
        int posX = screenWidth / 2;
        int posY = screenHeight / 2;
        
        bool success = false;
        
        switch (format) {
            case FORMAT_BMP:
                // Код для BMP
                {
                    uint8_t *fileData = (uint8_t *)malloc(fileSize);
                    if (!fileData) {
                        ::setUniversalFont();
                        M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
                        M5.Display.drawString("Out of memory", 20, 20);
                        file.close();
                        return;
                    }
                    
                    size_t bytesRead = file.read(fileData, fileSize);
                    if (bytesRead != fileSize) {
                        ::setUniversalFont();
                        M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
                        M5.Display.drawString("Error reading file", 20, 20);
                        free(fileData);
                        file.close();
                        return;
                    }
                    
                    int imgWidth = 0, imgHeight = 0;
                    bool dimensionSuccess = getBmpDimensions(fileData, fileSize, imgWidth, imgHeight);
                    
                    if (dimensionSuccess) {
                        // Calculate scaling factor to fit within screen
                        float scale = min((float)screenWidth / imgWidth, (float)screenHeight / imgHeight);
                        int scaledWidth = floor(imgWidth * scale);
                        int scaledHeight = floor(imgHeight * scale);
                        
                        // Ensure scaled dimensions fit within screen
                        scaledWidth = min(scaledWidth, screenWidth);
                        scaledHeight = min(scaledHeight, screenHeight);
                        
                        // Calculate position to center the image
                        posX = (screenWidth - scaledWidth) / 2;
                        posY = (screenHeight - scaledHeight) / 2;
                        
                        // Extract pixel data (24-bit BMP with 54-byte header)
                        const uint8_t* pixelData = fileData + 54;
                        
                        // Allocate buffer for scaled image
                        size_t bufferSize = scaledWidth * scaledHeight * sizeof(uint16_t);
                        uint16_t* scaledBuffer = (uint16_t*)malloc(bufferSize);
                        if (!scaledBuffer) {
                            ::setUniversalFont();
                            M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
                            M5.Display.drawString("Out of memory for scaling", 20, 20);
                            free(fileData);
                            file.close();
                            return;
                        }
                        
                        scaleBmpImage(pixelData, imgWidth, imgHeight, scaledBuffer, scaledWidth, scaledHeight);
                        // Draw scaled image
                        M5.Display.pushImage(posX, posY, scaledWidth, scaledHeight, scaledBuffer);
                        free(scaledBuffer);
                        success = true;
                    }
                    
                    free(fileData);
                }
                break;
                
            case FORMAT_JPG:
                {
                    // Получаем размеры изображения
                    int imgWidth, imgHeight;
                    if (getJpgDimensions(filename, imgWidth, imgHeight)) {
                        // Рассчитываем масштаб для вписывания в экран
                        float scale = min((float)screenWidth / imgWidth, (float)screenHeight / imgHeight);
                        int scaledWidth = floor(imgWidth * scale);
                        int scaledHeight = floor(imgHeight * scale);
                        
                        // Рассчитываем позицию для центрирования (верхний левый угол)
                        int jpgPosX = (screenWidth - scaledWidth) / 2;
                        int jpgPosY = (screenHeight - scaledHeight) / 2;
                        
                        // Отображаем JPG с масштабированием
                        success = M5.Display.drawJpgFile(filename.c_str(), jpgPosX, jpgPosY, scaledWidth, scaledHeight, 0, 0, 0, 0);
                    } else {
                        // Если не удалось получить размеры, пробуем отобразить напрямую
                        success = displayJpgImage(filename, posX, posY, screenWidth, screenHeight);
                    }
                }
                break;
                
            case FORMAT_PNG:
                {
                    // Получаем размеры изображения
                    int imgWidth, imgHeight;
                    if (getPngDimensions(filename, imgWidth, imgHeight)) {
                        // Рассчитываем масштаб для вписывания в экран
                        float scale = min((float)screenWidth / imgWidth, (float)screenHeight / imgHeight);
                        int scaledWidth = floor(imgWidth * scale);
                        int scaledHeight = floor(imgHeight * scale);
                        
                        // Рассчитываем позицию для центрирования (верхний левый угол)
                        int pngPosX = (screenWidth - scaledWidth) / 2;
                        int pngPosY = (screenHeight - scaledHeight) / 2;
                        
                        // Отображаем PNG с масштабированием
                        success = M5.Display.drawPngFile(filename.c_str(), pngPosX, pngPosY, scaledWidth, scaledHeight, 0, 0, 0, 0);
                    } else {
                        // Если не удалось получить размеры, пробуем отобразить напрямую
                        success = displayPngImage(filename, posX, posY, screenWidth, screenHeight);
                    }
                }
                break;
                
            default:
                success = false;
                break;
        }
        
        file.close();
        
        if (!success) {
            ::setUniversalFont();
            M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
            M5.Display.drawString("Error displaying image", 20, 20);
        }
    }

    void setupImgViewerButtons() {
        // Настройка кнопок для просмотра изображений
        FooterButton viewerFooterButtons[] = {
            {"Home", homeAction},
            {"Off", showOffScreen},
            {"Freeze", freezeAction},
            {"Files", filesAction}
        };
        footer.setButtons(viewerFooterButtons, 4);
    }
}
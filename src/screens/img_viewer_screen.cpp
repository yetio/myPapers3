// ./screens/img_viewer_screen.cpp
#include "img_viewer_screen.h"
#include "../ui.h"
#include "../sdcard.h"

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
        std::vector<FooterButton> viewerFooterButtons = {
            {"Home", homeAction},
            {"Off", showOffScreen},
            {"Freeze", freezeAction},
            {"Files", filesAction}
        };
        footer.setButtons(viewerFooterButtons);

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
        String fileLower = filename;
        fileLower.toLowerCase();

        if (!fileLower.endsWith(".bmp")) {
            M5.Display.setTextSize(2);
            M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
            M5.Display.drawString("Unsupported file format", frameLeft + 20, frameTop + 20);
            return;
        }

        // Open file
        File file = SD.open(filename, FILE_READ);
        if (!file) {
            M5.Display.setTextSize(2);
            M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
            M5.Display.drawString("Error opening file", frameLeft + 20, frameTop + 20);
            return;
        }

        // Read file data
        size_t fileSize = file.size();
        if (fileSize > 0) {
            // Check if file size is reasonable (e.g., max 8MB)
            if (fileSize > 8 * 1024 * 1024) {
                M5.Display.setTextSize(2);
                M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
                M5.Display.drawString("File too large", frameLeft + 20, frameTop + 20);
                file.close();
                return;
            }

            uint8_t *fileData = (uint8_t *)malloc(fileSize);
            if (!fileData) {
                M5.Display.setTextSize(2);
                M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
                M5.Display.drawString("Out of memory", frameLeft + 20, frameTop + 20);
                file.close();
                return;
            }

            size_t bytesRead = file.read(fileData, fileSize);
            if (bytesRead != fileSize) {
                M5.Display.setTextSize(2);
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
                float scale = min((float)(frameRight - frameLeft) / imgWidth, (float)(frameBottom - frameTop) / imgHeight);
                int scaledWidth = floor(imgWidth * scale);
                int scaledHeight = floor(imgHeight * scale);

                // Ensure scaled dimensions fit within frame
                scaledWidth = min(scaledWidth, frameRight - frameLeft);
                scaledHeight = min(scaledHeight, frameBottom - frameTop);

                // Calculate position to center the image
                int posX = frameLeft + ((frameRight - frameLeft) - scaledWidth) / 2;
                int posY = frameTop + ((frameBottom - frameTop) - scaledHeight) / 2;

                // Set clipping rectangle to frame boundaries
                M5.Display.setClipRect(frameLeft, frameTop, frameRight - frameLeft, frameBottom - frameTop);

                // Extract pixel data (24-bit BMP with 54-byte header)
                const uint8_t* pixelData = fileData + 54;

                // Allocate buffer for scaled image
                size_t bufferSize = scaledWidth * scaledHeight * sizeof(uint16_t);
                uint16_t* scaledBuffer = (uint16_t*)malloc(bufferSize);
                if (!scaledBuffer) {
                    M5.Display.setTextSize(2);
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

                // Reset clipping rectangle
                M5.Display.setClipRect(0, 0, EPD_WIDTH, EPD_HEIGHT);
            }

            free(fileData);
            file.close();
        }
    }

    void displayFullScreenImgFile(const String& filename) {
        String fileLower = filename;
        fileLower.toLowerCase();

        // Open file
        File file = SD.open(filename, FILE_READ);
        if (!file) {
            M5.Display.setTextSize(2);
            M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
            M5.Display.drawString("Error opening file", 20, 20);
            return;
        }

        // Read file data
        size_t fileSize = file.size();
        if (fileSize == 0 || fileSize > 8 * 1024 * 1024) {
            M5.Display.setTextSize(2);
            M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
            M5.Display.drawString("Invalid file size", 20, 20);
            file.close();
            return;
        }

        uint8_t *fileData = (uint8_t *)malloc(fileSize);
        if (!fileData) {
            M5.Display.setTextSize(2);
            M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
            M5.Display.drawString("Out of memory", 20, 20);
            file.close();
            return;
        }

        size_t bytesRead = file.read(fileData, fileSize);
        if (bytesRead != fileSize) {
            M5.Display.setTextSize(2);
            M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
            M5.Display.drawString("Error reading file", 20, 20);
            free(fileData);
            file.close();
            return;
        }

        int imgWidth = 0, imgHeight = 0;
        bool dimensionSuccess = getBmpDimensions(fileData, fileSize, imgWidth, imgHeight);

        if (dimensionSuccess) {
            // Calculate scaling factor to fit entire screen
            float scale = min((float)EPD_WIDTH / imgWidth, (float)EPD_HEIGHT / imgHeight);
            int scaledWidth = floor(imgWidth * scale);
            int scaledHeight = floor(imgHeight * scale);

            // Ensure scaled dimensions fit within screen
            scaledWidth = min(scaledWidth, EPD_WIDTH);
            scaledHeight = min(scaledHeight, EPD_HEIGHT);

            // Calculate position to center the image
            int posX = (EPD_WIDTH - scaledWidth) / 2;
            int posY = (EPD_HEIGHT - scaledHeight) / 2;

            // Set clipping rectangle to entire screen
            M5.Display.setClipRect(0, 0, EPD_WIDTH, EPD_HEIGHT);

            // Extract pixel data (24-bit BMP with 54-byte header)
            const uint8_t* pixelData = fileData + 54;

            // Allocate buffer for scaled image
            size_t bufferSize = scaledWidth * scaledHeight * sizeof(uint16_t);
            uint16_t* scaledBuffer = (uint16_t*)malloc(bufferSize);
            if (!scaledBuffer) {
                M5.Display.setTextSize(2);
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

            // Reset clipping rectangle
            M5.Display.setClipRect(0, 0, EPD_WIDTH, EPD_HEIGHT);
        }

        free(fileData);
        file.close();
    }

    void clearImgViewerScreen() {
        // Clear the entire screen
        M5.Display.fillScreen(TFT_WHITE);
        M5.Display.display();
    }

    String getCurrentImgFile() {
        return screens::currentImgOpened;
    }

    void setupImgViewerButtons() {
        // Buttons are handled in drawImgViewerScreen()
    }
}
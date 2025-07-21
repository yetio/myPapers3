#include "img_viewer_screen.h"
#include "../ui.h"
#include "../sdcard.h"
#include "../buttons/rotate.h"

enum ImageFormat {
    FORMAT_UNKNOWN,
    FORMAT_BMP
};




ImageFormat getImageFormat(const String& filename) {
    String ext = filename.substring(filename.lastIndexOf('.') + 1);
    ext.toLowerCase();
    
    if (ext == "bmp") return FORMAT_BMP;
    
    return FORMAT_UNKNOWN;
}





namespace screens {
    static String currentImgOpened = "";

    static constexpr int frameLeft = 0;
    static constexpr int frameTop = 100;
    static constexpr int frameRight = 540;
    static constexpr int frameBottom = 700;

    void drawImgViewerScreen(const String& filename) {
        currentImgOpened = filename;


        M5.Display.fillScreen(TFT_WHITE);


        M5.Display.drawRect(frameLeft, frameTop, frameRight - frameLeft, frameBottom - frameTop, TFT_BLACK);


        displayImgFile(filename);


        FooterButton viewerFooterButtons[] = {
            {"Home", homeAction},
            {"180°", rotateImg180Action},
            {"Freeze", freezeAction},
            {"Files", filesAction}
        };
        footer.setButtons(viewerFooterButtons, 4);


        ::drawRowsBuffered();

        footer.draw(footer.isVisible());
        M5.Display.display();
    }

    bool getBmpDimensions(const uint8_t* data, size_t size, int& width, int& height) {
        if (size < 26) return false;

        width = data[18] | (data[19] << 8) | (data[20] << 16) | (data[21] << 24);
        height = data[22] | (data[23] << 8) | (data[24] << 16) | (data[25] << 24);
        return true;
    }


    void scaleBmpImage(const uint8_t* srcData, int srcWidth, int srcHeight, uint16_t* destBuffer, int destWidth, int destHeight) {

        int rowSize = ((24 * srcWidth + 31) / 32) * 4;

        for(int y = 0; y < destHeight; y++) {
            for(int x = 0; x < destWidth; x++) {
                int srcX = x * srcWidth / destWidth;
                int srcY = srcHeight - 1 - (y * srcHeight / destHeight);


                srcX = constrain(srcX, 0, srcWidth - 1);
                srcY = constrain(srcY, 0, srcHeight - 1);

                int srcIndex = (srcY * rowSize) + (srcX * 3);

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
        

        File file = SD.open(filename, FILE_READ);
        if (!file) {
            ::setUniversalFont();
            M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
            M5.Display.drawString("Error opening file", frameLeft + 20, frameTop + 20);
            return;
        }
        

        size_t fileSize = file.size();
        if (fileSize == 0 || fileSize > 8 * 1024 * 1024) {
            ::setUniversalFont();
            M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
            M5.Display.drawString("Invalid file size", frameLeft + 20, frameTop + 20);
            file.close();
            return;
        }
        

        int frameWidth = frameRight - frameLeft;
        int frameHeight = frameBottom - frameTop;
        int posX = frameLeft + (frameWidth / 2);
        int posY = frameTop + (frameHeight / 2);
        

        M5.Display.setClipRect(frameLeft, frameTop, frameWidth, frameHeight);
        
        bool success = false;
        
        switch (format) {
            case FORMAT_BMP:

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

                        float scale = min((float)frameWidth / imgWidth, (float)frameHeight / imgHeight);
                        int scaledWidth = floor(imgWidth * scale);
                        int scaledHeight = floor(imgHeight * scale);
                        

                        scaledWidth = min(scaledWidth, frameWidth);
                        scaledHeight = min(scaledHeight, frameHeight);
                        

                        posX = frameLeft + (frameWidth - scaledWidth) / 2;
                        posY = frameTop + (frameHeight - scaledHeight) / 2;
                        

                        const uint8_t* pixelData = fileData + 54;
                        

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

                        M5.Display.pushImage(posX, posY, scaledWidth, scaledHeight, scaledBuffer);
                        free(scaledBuffer);
                        success = true;
                    }
                    
                    free(fileData);
                }
                break;
                

                
            default:
                success = false;
                break;
        }
        
        file.close();
        

        M5.Display.setClipRect(0, 0, EPD_WIDTH, EPD_HEIGHT);
        
        if (!success) {
            ::setUniversalFont();
            M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
            M5.Display.drawString("Error displaying image", frameLeft + 20, frameTop + 20);
        }
    }



    void clearImgViewerScreen() {

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
        

        File file = SD.open(filename, FILE_READ);
        if (!file) {
            ::setUniversalFont();
            M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
            M5.Display.drawString("Error opening file", 20, 20);
            return;
        }
        

        size_t fileSize = file.size();
        if (fileSize == 0 || fileSize > 8 * 1024 * 1024) {
            ::setUniversalFont();
            M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
            M5.Display.drawString("Invalid file size", 20, 20);
            file.close();
            return;
        }
        

        int screenWidth = EPD_WIDTH;
        int screenHeight = EPD_HEIGHT;
        int posX = screenWidth / 2;
        int posY = screenHeight / 2;
        
        bool success = false;
        
        switch (format) {
            case FORMAT_BMP:

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

                        float scale = min((float)screenWidth / imgWidth, (float)screenHeight / imgHeight);
                        int scaledWidth = floor(imgWidth * scale);
                        int scaledHeight = floor(imgHeight * scale);
                        

                        scaledWidth = min(scaledWidth, screenWidth);
                        scaledHeight = min(scaledHeight, screenHeight);
                        

                        posX = (screenWidth - scaledWidth) / 2;
                        posY = (screenHeight - scaledHeight) / 2;
                        

                        const uint8_t* pixelData = fileData + 54;
                        

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

                        M5.Display.pushImage(posX, posY, scaledWidth, scaledHeight, scaledBuffer);
                        free(scaledBuffer);
                        success = true;
                    }
                    
                    free(fileData);
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

        FooterButton viewerFooterButtons[] = {
            {"Home", homeAction},
            {"180°", rotateImg180Action},
            {"Freeze", freezeAction},
            {"Files", filesAction}
        };
        footer.setButtons(viewerFooterButtons, 4);
    }

    void setupImgViewerRotateButtons() {

        FooterButton rotateButtons[] = {
            {"Home", homeAction},
            {"180°", rotateImg180Action},
            {"Freeze", freezeAction},
            {"Files", filesAction}
        };
        footer.setButtons(rotateButtons, 4);
    }
}
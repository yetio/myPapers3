// ./screens/txt_viewer_screen.cpp
#include "txt_viewer_screen.h"
#include "../ui.h"
#include "../sdcard.h"

namespace screens {
    static String currentFileOpened = "";

    void drawTxtViewerScreen(const String& filename) {
        // Устанавливаем универсальный шрифт
        ::setUniversalFont();
        
        currentFileOpened = filename;
        // Clear all rows except headers
        for (int row = 2; row <= 14; ++row) { // Use rows 2-14
            ::bufferRow("", row);
        }

        ::bufferRow("Text Viewer: " + filename, 2); // Header for viewer with filename

        displayTxtFile(filename);

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

    void displayTxtFile(const String& filename) {
        File file = SD.open(filename);
        if (!file) {
            ::bufferRow("Failed to open file", 3);
            return;
        }

        int row = 3;
        const int maxRowsAvailable = 13; // От 3 до 13 (всего 11 строк)
        
        while (file.available() && row <= maxRowsAvailable) {
            String line = file.readStringUntil('\n');
            line.trim(); // Remove leading and trailing whitespace
            if (line.length() > 0) { // Skip empty lines
                // Получаем размеры строки
                RowPosition pos = getRowPosition(row);
                int maxWidth = pos.width - 20; // 10px с каждой стороны
                
                // Разбиваем длинную строку на несколько строк с учетом переноса слов
                const int MAX_WRAPPED_LINES = 10;
                String wrappedLines[MAX_WRAPPED_LINES];
                int wrappedLineCount = 0;
                ::wordWrap(line, maxWidth, wrappedLines, wrappedLineCount, MAX_WRAPPED_LINES);
                
                // Отображаем каждую строку
                for (int i = 0; i < wrappedLineCount; i++) {
                    const String& wrappedLine = wrappedLines[i];
                    if (row <= maxRowsAvailable) {
                        ::bufferRow(wrappedLine, row);
                        row++;
                    } else {
                        break; // Достигли максимального количества строк
                    }
                }
            }
        }

        file.close();
    }

    void displayFullScreenFile(const String& filename) {
        // Устанавливаем универсальный шрифт
        ::setUniversalFont();
        
        File file = SD.open(filename);
        if (!file) {
            ::bufferRow("Failed to open file", 3);
            return;
        }

        // Clear all rows
        for (int row = 2; row <= 14; ++row) {
            ::bufferRow("", row);
        }

        ::bufferRow("Full Screen Text", 2);

        int row = 3;
        const int maxRowsAvailable = 13; // От 3 до 13 (всего 11 строк)
        
        while (file.available() && row <= maxRowsAvailable) {
            String line = file.readStringUntil('\n');
            line.trim(); // Remove leading and trailing whitespace
            if (line.length() > 0) { // Skip empty lines
                // Получаем размеры строки
                RowPosition pos = getRowPosition(row);
                int maxWidth = pos.width - 20; // 10px с каждой стороны
                
                // Разбиваем длинную строку на несколько строк с учетом переноса слов
                const int MAX_WRAPPED_LINES = 10;
                String wrappedLines[MAX_WRAPPED_LINES];
                int wrappedLineCount = 0;
                ::wordWrap(line, maxWidth, wrappedLines, wrappedLineCount, MAX_WRAPPED_LINES);
                
                // Отображаем каждую строку
                for (int i = 0; i < wrappedLineCount; i++) {
                    const String& wrappedLine = wrappedLines[i];
                    if (row <= maxRowsAvailable) {
                        ::bufferRow(wrappedLine, row);
                        row++;
                    } else {
                        break; // Достигли максимального количества строк
                    }
                }
            }
        }

        file.close();

        // Render all buffered rows
        ::drawRowsBuffered();
        M5.Display.display();
    }

    String getCurrentTxtFile() {
        return currentFileOpened;
    }

    void setupFreezeButton() {
        // Buttons are handled in drawTxtViewerScreen()
    }
}
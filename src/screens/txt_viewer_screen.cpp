// ./screens/txt_viewer_screen.cpp
#include "txt_viewer_screen.h"
#include "../ui.h"
#include "../sdcard.h"

namespace screens {
    static String currentFileOpened = "";

    void drawTxtViewerScreen(const String& filename) {
        currentFileOpened = filename;
        // Clear all rows except headers
        for (int row = 2; row <= 14; ++row) { // Use rows 2-14
            ::bufferRow("", row);
        }

        ::bufferRow("Text Viewer: " + filename, 2); // Header for viewer with filename

        displayTxtFile(filename);

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

    void displayTxtFile(const String& filename) {
        File file = SD.open(filename);
        if (!file) {
            ::bufferRow("Failed to open file", 3);
            return;
        }

        int row = 3;
        while (file.available() && row < 14) { // Adjusted rows for headers
            String line = file.readStringUntil('\n');
            line.trim(); // Remove leading and trailing whitespace
            if (line.length() > 0) { // Skip empty lines
                ::bufferRow(line, row);
                row++;
            }
        }

        file.close();
    }

    void displayFullScreenFile(const String& filename) {
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
        while (file.available() && row < 14) { // Fill up to row 13
            String line = file.readStringUntil('\n');
            line.trim(); // Remove leading and trailing whitespace
            if (line.length() > 0) { // Skip empty lines
                ::bufferRow(line, row);
                row++;
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
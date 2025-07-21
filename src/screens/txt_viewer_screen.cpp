#include "txt_viewer_screen.h"
#include "../ui.h"
#include "../sdcard.h"
#include "../buttons/rotate.h"

namespace screens {
    static String currentFileOpened = "";

    void drawTxtViewerScreen(const String& filename) {

        ::setUniversalFont();
        
        currentFileOpened = filename;

        for (int row = 2; row <= 14; ++row) {
            ::bufferRow("", row);
        }

        ::bufferRow("Text Viewer: " + filename, 2);

        displayTxtFile(filename);


        FooterButton viewerFooterButtons[] = {
            {"Home", homeAction},
            {"180°", rotateTxt180Action},
            {"Freeze", freezeAction},
            {"Files", filesAction}
        };
        footer.setButtons(viewerFooterButtons, 4);


        ::drawRowsBuffered();

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
        const int maxRowsAvailable = 13;
        
        while (file.available() && row <= maxRowsAvailable) {
            String line = file.readStringUntil('\n');
            line.trim();
            if (line.length() > 0) {

                RowPosition pos = getRowPosition(row);
                int maxWidth = pos.width - 20;
                

                const int MAX_WRAPPED_LINES = 10;
                String wrappedLines[MAX_WRAPPED_LINES];
                int wrappedLineCount = 0;
                ::wordWrap(line, maxWidth, wrappedLines, wrappedLineCount, MAX_WRAPPED_LINES);
                

                for (int i = 0; i < wrappedLineCount; i++) {
                    const String& wrappedLine = wrappedLines[i];
                    if (row <= maxRowsAvailable) {
                        ::bufferRow(wrappedLine, row);
                        row++;
                    } else {
                        break;
                    }
                }
            }
        }

        file.close();
    }

    void displayFullScreenFile(const String& filename) {

        ::setUniversalFont();
        
        File file = SD.open(filename);
        if (!file) {
            ::bufferRow("Failed to open file", 3);
            return;
        }


        for (int row = 2; row <= 14; ++row) {
            ::bufferRow("", row);
        }

        ::bufferRow("Full Screen Text", 2);

        int row = 3;
        const int maxRowsAvailable = 13;
        
        while (file.available() && row <= maxRowsAvailable) {
            String line = file.readStringUntil('\n');
            line.trim();
            if (line.length() > 0) {

                RowPosition pos = getRowPosition(row);
                int maxWidth = pos.width - 20;
                

                const int MAX_WRAPPED_LINES = 10;
                String wrappedLines[MAX_WRAPPED_LINES];
                int wrappedLineCount = 0;
                ::wordWrap(line, maxWidth, wrappedLines, wrappedLineCount, MAX_WRAPPED_LINES);
                

                for (int i = 0; i < wrappedLineCount; i++) {
                    const String& wrappedLine = wrappedLines[i];
                    if (row <= maxRowsAvailable) {
                        ::bufferRow(wrappedLine, row);
                        row++;
                    } else {
                        break;
                    }
                }
            }
        }

        file.close();


        ::drawRowsBuffered();
        M5.Display.display();
    }

    String getCurrentTxtFile() {
        return currentFileOpened;
    }

    void setupFreezeButton() {

    }

    void setupTxtViewerRotateButtons() {

        FooterButton rotateButtons[] = {
            {"Home", homeAction},
            {"180°", rotateTxt180Action},
            {"Freeze", freezeAction},
            {"Files", filesAction}
        };
        footer.setButtons(rotateButtons, 4);
    }
}
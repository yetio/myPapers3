// ./screens/files_screen.cpp
#include "files_screen.h"
#include "../ui.h"
#include "../sdcard.h"
#include <algorithm>

// Variables for pagination
static int currentPage = 0;
static const int itemsPerPage = 9; // Используем строки 5-13 для файлов (9 строк)
static int totalPages = 1; // Initialize total pages

namespace screens {
    // Custom clamp function if std::clamp is unavailable
    template <typename T>
    T customClamp(T value, T minVal, T maxVal) {
        return (value < minVal) ? minVal : (value > maxVal) ? maxVal : value;
    }

    // Helper function to get last folder name
    String getLastFolder(const String& path) {
        if (path == "/") return "/";
        String cleanPath = path;
        if (cleanPath.endsWith("/")) {
            cleanPath = cleanPath.substring(0, cleanPath.length() - 1);
        }
        int lastSlash = cleanPath.lastIndexOf('/');
        if (lastSlash >= 0) {
            return cleanPath.substring(lastSlash + 1);
        }
        return cleanPath;
    }

    // Reset pagination (only when navigating directories)
    void resetPagination() {
        currentPage = 0;
    }

    void drawFilesScreen() {
        // Do not reset pagination here
        // resetPagination(); // Removed to allow pagination to work

        displayedFilesCount = 0;
        for (int i = 0; i < MAX_DISPLAYED_FILES; i++) {
            displayedFiles[i] = "";
        }
        bufferRow("Files Manager", 2, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
        String lastFolder = getLastFolder(currentPath);
        bufferRow("Path: " + lastFolder, 3, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);

        // Add "..." for navigation if not root, otherwise a blank row
        if (currentPath != "/") {
            bufferRow("...", 4, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true); // Underline enabled to show it's clickable
        } else {
            bufferRow("", 4, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
        }

        // Clear remaining rows
        for (int row = 5; row <= 13; ++row) { // Use rows 5-13 for items (9 rows)
            bufferRow("", row, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
        }

        // Load files and sort them
        String pathToOpen = currentPath;
        if (pathToOpen.length() > 1 && pathToOpen.endsWith("/")) {
            pathToOpen = pathToOpen.substring(0, pathToOpen.length() - 1);
        }
        File root = SD.open(pathToOpen);

        if (!root) {
            bufferRow("No files found", 5, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
            return;
        }

        static String folders[MAX_DISPLAYED_FILES];
        static String files[MAX_DISPLAYED_FILES];
        int foldersCount = 0;
        int filesCount = 0;
        const size_t maxFilesPerBatch = 100; // Process more files per batch for better performance
        size_t processedFiles = 0;

        while (true) {
            File file = root.openNextFile();
            if (!file) break;

            // Process files in batches to prevent memory exhaustion
            if (processedFiles >= maxFilesPerBatch) {
                displayMessage("Loading more files...");
                M5.Display.display();
                processedFiles = 0;
            }

            String filename = file.name();
            if (filename.startsWith(".")) {
                file.close();
                continue; // Skip hidden/system files
            }

            if (file.isDirectory()) {
                if (foldersCount < MAX_DISPLAYED_FILES) {
                    folders[foldersCount++] = filename + "/";
                }
            } else {
                if (filesCount < MAX_DISPLAYED_FILES) {
                    files[filesCount++] = filename;
                }
            }
            file.close();
            processedFiles++;
        }
        root.close();

        // Simple bubble sort for small arrays
        for (int i = 0; i < foldersCount - 1; i++) {
            for (int j = 0; j < foldersCount - i - 1; j++) {
                if (folders[j] > folders[j + 1]) {
                    String temp = folders[j];
                    folders[j] = folders[j + 1];
                    folders[j + 1] = temp;
                }
            }
        }

        for (int i = 0; i < filesCount - 1; i++) {
            for (int j = 0; j < filesCount - i - 1; j++) {
                if (files[j] > files[j + 1]) {
                    String temp = files[j];
                    files[j] = files[j + 1];
                    files[j + 1] = temp;
                }
            }
        }

        displayedFilesCount = 0;
        // Copy folders first
        for (int i = 0; i < foldersCount && displayedFilesCount < MAX_DISPLAYED_FILES; i++) {
            displayedFiles[displayedFilesCount++] = folders[i];
        }
        // Copy files
        for (int i = 0; i < filesCount && displayedFilesCount < MAX_DISPLAYED_FILES; i++) {
            displayedFiles[displayedFilesCount++] = files[i];
        }

        // Calculate total pages
        totalPages = (displayedFilesCount + itemsPerPage - 1) / itemsPerPage;

        // Ensure currentPage is within bounds
        if (currentPage >= totalPages) {
            currentPage = totalPages - 1;
        }
        if (currentPage < 0) {
            currentPage = 0;
        }

        // Display files and folders for the current page
        int startIdx = currentPage * itemsPerPage;
        int endIdx = std::min(startIdx + itemsPerPage, displayedFilesCount);
        for (int i = startIdx; i < endIdx; ++i) {
            bool isFolder = displayedFiles[i].endsWith("/");
            bufferRow(displayedFiles[i], 5 + (i - startIdx), TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true); // Underline enabled
        }

        // Add pagination controls with <<<--   <--   [currentPage + 1]   -->   -->>>
        if (totalPages > 1) {
            // Define pagination buttons
            String paginationButtons[] = {"<<<--", "<--", String(currentPage + 1), "-->", "-->>>"};  
            int numButtons = 5; // Now 5 buttons including the page number
            int sectionWidth = EPD_WIDTH / numButtons;
            RowPosition pos = getRowPosition(14); // Move above footer (row 14)
            int underlineY = pos.y + 30;

            for(int i = 0; i < numButtons; ++i){
                String btn = paginationButtons[i];
                int btnX = pos.x + i * sectionWidth + 10; // 10px padding
                // Center text vertically in the row
                int btnY = pos.y + (pos.height - 20) / 2; // Center vertically (20 is approximate text height)
                M5.Display.setCursor(btnX, btnY);
                M5.Display.print(btn);
                // Draw underline for each button
                int textWidth = M5.Display.textWidth(btn.c_str());
                int centeredUnderlineY = pos.y + pos.height - 10; // Position underline near bottom of row
                M5.Display.drawLine(btnX, centeredUnderlineY, btnX + textWidth, centeredUnderlineY, TFT_BLACK);
            }
        }
    }

    void handlePagination(String button) {
        if (button == "<<<--") {
            if (currentPage != 0) {
                currentPage = 0;
                displayMessage("First page pressed");
                renderCurrentScreen();
                clearMessage();
            }
        }
        else if (button == "<--") {
            if (currentPage > 0) {
                currentPage--;
                displayMessage("Previous page pressed");
                renderCurrentScreen();
                clearMessage();
            }
        }
        else if (button == "-->") {
            if (currentPage < totalPages - 1) {
                currentPage++;
                displayMessage("Next page pressed");
                renderCurrentScreen();
                clearMessage();
            }
        }
        else if (button == "-->>>") {
            if (currentPage != totalPages - 1) {
                currentPage = totalPages - 1;
                displayMessage("Last page pressed");
                renderCurrentScreen();
                clearMessage();
            }
        }
    }

    void selectFile(const String& filename) {
        if (filename == "...") {
            navigateUp();
        } else if (filename.endsWith("/")) {
            navigateTo(currentPath + filename);
        } else {
            // Determine file type and navigate accordingly
            if (filename.endsWith(".txt")) {
                navigateTo(currentPath + filename);
                currentScreen = TXT_VIEWER_SCREEN;
                renderCurrentScreen();
            }
            // Поддержка различных форматов изображений (без GIF)
            else if (filename.endsWith(".bmp") || 
                     filename.endsWith(".jpg") || 
                     filename.endsWith(".jpeg") || 
                     filename.endsWith(".png")) {
                navigateTo(currentPath + filename);
                currentScreen = IMG_VIEWER_SCREEN;
                renderCurrentScreen();
            }
            else {
                // Unsupported file type
                displayMessage("Unsupported file type");
                clearMessage();
            }
        }
    }

    void handleTouch(int touchRow, int touchX, int touchY) {
        if (currentScreen == FILES_SCREEN) {
            if (touchRow >= 5 && touchRow <= 13) { // Handle rows 5-13 (9 rows)
                int index = currentPage * itemsPerPage + (touchRow - 5);
                if (index < displayedFilesCount) {
                    String selected = displayedFiles[index];
                    selectFile(selected);
                }
            } else if (touchRow == 4) { // Handle "..."
                selectFile("...");
            } else if (touchRow == 14) { // Handle pagination on row 14 (above footer)
                int width = EPD_WIDTH;

                // Define pagination button sizes and positions with 50% increased touch area
                // Each section is width / 5 (now 5 buttons)
                int sectionWidth = width / 5; // 108 for width=540

                // Calculate touch regions with 50% increased width (1.5x)
                // Original touchWidth = sectionWidth
                // Increased touchWidth = 1.5 * sectionWidth = 162
                // Touch regions centered in each section

                // "<<<--"
                int firstBtnCenterX = sectionWidth / 2; // 54
                int firstBtnStartX = customClamp(firstBtnCenterX - (int)(sectionWidth * 0.75), 0, width);
                int firstBtnEndX = customClamp(firstBtnCenterX + (int)(sectionWidth * 0.75), 0, width);

                // "<--"
                int secondBtnCenterX = 3 * sectionWidth / 2; // 162
                int secondBtnStartX = customClamp(secondBtnCenterX - (int)(sectionWidth * 0.75), 0, width);
                int secondBtnEndX = customClamp(secondBtnCenterX + (int)(sectionWidth * 0.75), 0, width);

                // Page number (not clickable)
                int thirdBtnCenterX = 5 * sectionWidth / 2; // 270
                int thirdBtnStartX = customClamp(thirdBtnCenterX - (int)(sectionWidth * 0.75), 0, width);
                int thirdBtnEndX = customClamp(thirdBtnCenterX + (int)(sectionWidth * 0.75), 0, width);

                // "-->"
                int fourthBtnCenterX = 7 * sectionWidth / 2; // 378
                int fourthBtnStartX = customClamp(fourthBtnCenterX - (int)(sectionWidth * 0.75), 0, width);
                int fourthBtnEndX = customClamp(fourthBtnCenterX + (int)(sectionWidth * 0.75), 0, width);

                // "-->>>"
                int fifthBtnCenterX = 9 * sectionWidth / 2; // 486
                int fifthBtnStartX = customClamp(fifthBtnCenterX - (int)(sectionWidth * 0.75), 0, width);
                int fifthBtnEndX = customClamp(fifthBtnCenterX + (int)(sectionWidth * 0.75), 0, width);

                // Check which button was pressed
                if (touchX >= firstBtnStartX && touchX <= firstBtnEndX) {
                    handlePagination("<<<--");
                }
                else if (touchX >= secondBtnStartX && touchX <= secondBtnEndX) {
                    handlePagination("<--");
                }
                else if (touchX >= fourthBtnStartX && touchX <= fourthBtnEndX) {
                    handlePagination("-->");
                }
                else if (touchX >= fifthBtnStartX && touchX <= fifthBtnEndX) {
                    handlePagination("-->>>");
                }
            }
        }
    }
}
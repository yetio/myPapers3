#include "apps_screen.h"
#include "../ui.h"
#include <vector>
#include <String>

// Include test app headers
#include "../apps/test/app_screen.h"
#include "../apps/test2/app_screen.h"

namespace screens {

    // Static list of applications
    const std::vector<String> installedApps = {"test", "test2"}; // Hardcoded list based on src/apps

    void drawAppsScreen() {
        ::updateHeader(); // Update header for this screen

        // Clear content area
        RowPosition contentStart = getRowPosition(2);
        RowPosition footerStart = getRowPosition(15);
        M5.Display.fillRect(contentStart.x, contentStart.y, contentStart.width, footerStart.y - contentStart.y, TFT_WHITE);

        // Use the static list of apps
        if (installedApps.empty()) {
            ::bufferRow("No applications found.", 2, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
        } else {
            ::bufferRow("Applications:", 2, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
            for (size_t i = 0; i < installedApps.size(); ++i) {
                // Display each app name from the static list, starting from row 3
                ::bufferRow(installedApps[i], i + 3, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true); // Make app names clickable
            }
        }
        // ::drawRowsBuffered(); // Drawing is handled in renderCurrentScreen
    }

    void handleAppsSelection(int selectedRow) {
         // Use the static list of apps
         if (selectedRow - 3 >= 0 && selectedRow - 3 < installedApps.size()) {
            String selectedApp = installedApps[selectedRow - 3];
            
            // Check which app was selected and call its drawing function
            if (selectedApp == "test") {
                ::displayMessage("Launching test app...");
                // Set current screen to TEST_APP_SCREEN
                ::currentScreen = TEST_APP_SCREEN;
                ::renderCurrentScreen(); // Re-render to show app screen
            } else if (selectedApp == "test2") {
                ::displayMessage("Launching test2 app...");
                // Set current screen to TEST2_APP_SCREEN
                ::currentScreen = TEST2_APP_SCREEN;
                ::renderCurrentScreen(); // Re-render to show app screen
            } else {
                // This case should not be reached with a static list unless there's an index issue,
                // but as a fallback, display a message.
                ::displayMessage("Unknown application selected.");
                ::renderCurrentScreen(); // Re-render to show message
            }
         }
    }
} 
#include "apps_screen.h"
#include "../ui.h"
#include <String>


#include "../apps/text_lang_test/app_screen.h"
#include "../apps/test2/app_screen.h"
#include "../apps/geometry_test/app_screen.h"
#include "../apps/swipe_test/app_screen.h"
#include "../apps/reader/app_screen.h"

namespace screens {

    const String installedApps[] = {"text_lang_test", "test2", "geometry_test", "swipe_test", "reader"};
    const int installedAppsCount = 5;

    void drawAppsScreen() {
        ::updateHeader();


        RowPosition contentStart = getRowPosition(2);
        RowPosition footerStart = getRowPosition(15);
        M5.Display.fillRect(contentStart.x, contentStart.y, contentStart.width, footerStart.y - contentStart.y, TFT_WHITE);


        if (installedAppsCount == 0) {
            ::bufferRow("No applications found.", 2, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
        } else {
            ::bufferRow("Applications:", 2, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
            for (int i = 0; i < installedAppsCount; ++i) {
                ::bufferRow(installedApps[i], i + 3, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true);
            }
        }

    }

    void handleAppsSelection(int selectedRow) {

         if (selectedRow - 3 >= 0 && selectedRow - 3 < installedAppsCount) {
            String selectedApp = installedApps[selectedRow - 3];
            

            if (selectedApp == "text_lang_test") {
                ::displayMessage("Text Language Font Test app");
                ::currentScreen = TEXT_LANG_TEST_SCREEN;
                ::renderCurrentScreen();
            } else if (selectedApp == "test2") {
                ::displayMessage("Launching test2 app...");
                ::currentScreen = TEST2_APP_SCREEN;
                ::renderCurrentScreen();
            } else if (selectedApp == "geometry_test") {
                ::displayMessage("Launching geometry test app...");
                ::currentScreen = GEOMETRY_TEST_SCREEN;
                ::renderCurrentScreen();
            } else if (selectedApp == "swipe_test") {
                ::displayMessage("Launching swipe test app...");
                ::currentScreen = SWIPE_TEST_SCREEN;
                ::renderCurrentScreen();
            } else if (selectedApp == "reader") {
                ::currentScreen = READER_APP_SCREEN;
                ::renderCurrentScreen();
            } else {
                ::displayMessage("Unknown application selected.");
                ::renderCurrentScreen();
            }
         }
    }
}
#include "config_screen.h"
#include "../ui.h"
#include "../settings.h"
#include "../keyboards/eng_keyboard.h"

namespace screens {

    enum class ConfigScreenState { LIST, EDIT_TODOIST, EDIT_ICAL };
    static ConfigScreenState currentState = ConfigScreenState::LIST;
    static String inputBuffer = "";

    void drawConfigScreen() {
        if (currentState == ConfigScreenState::EDIT_TODOIST) {
            ::bufferRow("Enter Todoist Token:", 2, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true);
            ::bufferRow("Token: " + inputBuffer, 3, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
            keyboards::drawEngKeyboard();
        } else if (currentState == ConfigScreenState::EDIT_ICAL) {
            ::bufferRow("Enter iCal URL:", 2, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true);
            ::bufferRow("URL: " + inputBuffer, 3, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
            keyboards::drawEngKeyboard();
        } else {
            ::bufferRow("Configuration", 2, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true);
            Settings settings;
            ::bufferRow("WiFi: Use WiFi screen", 3, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true);
            ::bufferRow(String("Todoist Token: ") + (settings.getTodoistToken().length() ? "(set)" : "(not set)"), 4, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true);
            ::bufferRow(String("iCal URL: ") + (settings.getICalUrl().length() ? "(set)" : "(not set)"), 5, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true);
            for (int row = 6; row <= 13; ++row) {
                ::bufferRow("", row, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
            }
        }
    }

    void handleConfigKeyboardInput(String key) {
        if (key == "<") {
            if (inputBuffer.length() > 0) {
                inputBuffer.remove(inputBuffer.length() - 1);
            }
        } else if (key == "~") {
            keyboards::toggleKeyboardState();
        } else if (key == ">") {
            // Save input
            Settings settings;
            if (currentState == ConfigScreenState::EDIT_TODOIST) {
                settings.setTodoistToken(inputBuffer);
                displayMessage("Todoist token saved");
            } else if (currentState == ConfigScreenState::EDIT_ICAL) {
                settings.setICalUrl(inputBuffer);
                displayMessage("iCal URL saved");
            }
            currentState = ConfigScreenState::LIST;
            inputBuffer = "";
        } else {
            inputBuffer += key;
        }
        renderCurrentScreen();
    }

    void handleConfigTouch(int row, int x, int y) {
        if (currentState == ConfigScreenState::EDIT_TODOIST || currentState == ConfigScreenState::EDIT_ICAL) {
            String key = keyboards::getKeyFromTouch(x, y);
            if (!key.isEmpty()) {
                handleConfigKeyboardInput(key);
            }
            return;
        }

        if (row == 3) {
            displayMessage("Use WiFi screen for WiFi config");
        } else if (row == 4) {
            Settings settings;
            inputBuffer = settings.getTodoistToken();
            currentState = ConfigScreenState::EDIT_TODOIST;
            renderCurrentScreen();
        } else if (row == 5) {
            Settings settings;
            inputBuffer = settings.getICalUrl();
            currentState = ConfigScreenState::EDIT_ICAL;
            renderCurrentScreen();
        }
    }

}

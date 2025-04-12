#include "eng_keyboard.h"
#include "../ui.h"

namespace keyboards {
    void drawEngKeyboard() {
        // Определение размеров клавиатуры
        const int keyWidth = EPD_WIDTH / 11; // 11 кнопок в строке для размещения backspace
        const int keyHeight = 60; // Фиксированная высота для каждой кнопки
        const int startY = EPD_HEIGHT - (4 * keyHeight) - 60; // Позиция выше футера (строка 15)

        // Очистка области клавиатуры
        M5.Display.fillRect(0, startY, EPD_WIDTH, 4 * keyHeight, TFT_WHITE);

        // Отрисовка каждой кнопки
        for (size_t row = 0; row < ENG_KEYBOARD_LAYOUT.size(); ++row) {
            for (size_t col = 0; col < ENG_KEYBOARD_LAYOUT[row].size(); ++col) {
                int x = col * keyWidth;
                int y = startY + row * keyHeight;

                // No background highlight for any keys

                // Отрисовка текста кнопки
                M5.Display.setCursor(x + (keyWidth - M5.Display.textWidth(ENG_KEYBOARD_LAYOUT[row][col].c_str())) / 2,
                                   y + (keyHeight - M5.Display.fontHeight()) / 2);
                M5.Display.setTextColor(TFT_BLACK);
                M5.Display.setTextSize(2);
                M5.Display.print(ENG_KEYBOARD_LAYOUT[row][col]);
            }
        }

        // Обновление дисплея
        M5.Display.display();
    }
}
#include "app_screen.h"
#include "../../ui.h"

namespace apps_text_lang_test {
    // Тестовая фраза на разных языках
    static const String testPhraseEn = "This is a test phrase for font display.";
    static const String testPhraseRu = "Это тестовая фраза для отображения шрифта.";
    static const String testPhraseJa = "これはフォント表示のためのテストフレーズです。";
    static const String testPhraseZh = "这是用于字体显示的测试短语。";
    
    // Функция для отображения экрана приложения
    void drawAppScreen() {
        // Сохраняем текущие настройки шрифта
        const lgfx::IFont* currentFont = M5.Display.getFont();
        
        ::updateHeader(); // Обновляем заголовок для этого экрана

        // Очищаем область контента
        RowPosition contentStart = getRowPosition(2);
        RowPosition footerStart = getRowPosition(15);
        M5.Display.fillRect(contentStart.x, contentStart.y, contentStart.width, footerStart.y - contentStart.y, TFT_WHITE);

        // Устанавливаем универсальный шрифт
        ::setUniversalFont();
        
        // Отображаем заголовок приложения с фиксированным размером шрифта
        M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
        M5.Display.setTextDatum(top_left);
        M5.Display.setTextSize(FONT_SIZE_ALL); // Используем стандартный размер для интерфейса
        M5.Display.drawString("Text Language Font Test", 10, contentStart.y + 10);
        
        // Фиксированный размер шрифта для всех тестовых фраз
        float testFontSize = FONT_SIZE_ALL;
        
        // Отображаем тестовые фразы на разных языках
        int y_pos = contentStart.y + 80;
        int y_spacing = 80; // Расстояние между блоками фраз
        int maxWidth = contentStart.width - 20; // Ширина для текста с учетом отступов
        
        // Фиксированное расстояние между строками
        float lineSpacing = 40;
        
        // Применяем перенос строк к тестовым фразам и отображаем их
        M5.Display.setTextSize(testFontSize);
        
        // Английский
        std::vector<String> wrappedEn = ::wordWrap(testPhraseEn, maxWidth / testFontSize * FONT_SIZE_ALL);
        for (size_t i = 0; i < wrappedEn.size(); i++) {
            M5.Display.drawString(wrappedEn[i], 10, y_pos + i * lineSpacing);
        }
        // Рассчитываем позицию для следующего блока текста с учётом количества строк
        y_pos += y_spacing + (wrappedEn.size() > 1 ? (wrappedEn.size() - 1) * lineSpacing : 0);
        
        // Русский
        std::vector<String> wrappedRu = ::wordWrap(testPhraseRu, maxWidth / testFontSize * FONT_SIZE_ALL);
        for (size_t i = 0; i < wrappedRu.size(); i++) {
            M5.Display.drawString(wrappedRu[i], 10, y_pos + i * lineSpacing);
        }
        // Рассчитываем позицию для следующего блока текста с учётом количества строк
        y_pos += y_spacing + (wrappedRu.size() > 1 ? (wrappedRu.size() - 1) * lineSpacing : 0);
        
        // Японский
        std::vector<String> wrappedJa = ::wordWrap(testPhraseJa, maxWidth / testFontSize * FONT_SIZE_ALL);
        for (size_t i = 0; i < wrappedJa.size(); i++) {
            M5.Display.drawString(wrappedJa[i], 10, y_pos + i * lineSpacing);
        }
        // Рассчитываем позицию для следующего блока текста с учётом количества строк
        y_pos += y_spacing + (wrappedJa.size() > 1 ? (wrappedJa.size() - 1) * lineSpacing : 0);
        
        // Китайский
        std::vector<String> wrappedZh = ::wordWrap(testPhraseZh, maxWidth / testFontSize * FONT_SIZE_ALL);
        for (size_t i = 0; i < wrappedZh.size(); i++) {
            M5.Display.drawString(wrappedZh[i], 10, y_pos + i * lineSpacing);
        }
        
        // Восстанавливаем исходные настройки шрифта
        M5.Display.setFont(currentFont);
        M5.Display.setTextSize(1.0); // Возвращаем стандартный размер
    }
} 
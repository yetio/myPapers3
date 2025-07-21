#include "app_screen.h"
#include "../../ui.h"

namespace apps_text_lang_test {
    
    static const String testPhraseEn = "This is a test phrase for font display.";
    static const String testPhraseRu = "Это тестовая фраза для отображения шрифта.";
    static const String testPhraseJa = "これはフォント表示のためのテストフレーズです。";
    static const String testPhraseZh = "这是用于字体显示的测试短语。";
    
    
    void drawAppScreen() {
    
        const lgfx::IFont* currentFont = M5.Display.getFont();
        
        ::updateHeader();

    
        RowPosition contentStart = getRowPosition(2);
        RowPosition footerStart = getRowPosition(15);
        M5.Display.fillRect(contentStart.x, contentStart.y, contentStart.width, footerStart.y - contentStart.y, TFT_WHITE);

    
        ::setUniversalFont();
        
    
        M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
        M5.Display.setTextDatum(top_left);
        M5.Display.setTextSize(FONT_SIZE_ALL);
        M5.Display.drawString("Text Language Font Test", 10, contentStart.y + 10);
        
    
        float testFontSize = FONT_SIZE_ALL;
        
    
        int y_pos = contentStart.y + 80;
        int y_spacing = 80;
        int maxWidth = contentStart.width - 20;
        

        float lineSpacing = 40;
        

        M5.Display.setTextSize(testFontSize);
        

        String wrappedEn[10];
        int wrappedEnCount = 0;
        ::wordWrap(testPhraseEn, maxWidth / testFontSize * FONT_SIZE_ALL, wrappedEn, wrappedEnCount, 10);
        for (int i = 0; i < wrappedEnCount; i++) {
            M5.Display.drawString(wrappedEn[i], 10, y_pos + i * lineSpacing);
        }

        y_pos += y_spacing + (wrappedEnCount > 1 ? (wrappedEnCount - 1) * lineSpacing : 0);
        

        String wrappedRu[10];
        int wrappedRuCount = 0;
        ::wordWrap(testPhraseRu, maxWidth / testFontSize * FONT_SIZE_ALL, wrappedRu, wrappedRuCount, 10);
        for (int i = 0; i < wrappedRuCount; i++) {
            M5.Display.drawString(wrappedRu[i], 10, y_pos + i * lineSpacing);
        }

        y_pos += y_spacing + (wrappedRuCount > 1 ? (wrappedRuCount - 1) * lineSpacing : 0);
        

        String wrappedJa[10];
        int wrappedJaCount = 0;
        ::wordWrap(testPhraseJa, maxWidth / testFontSize * FONT_SIZE_ALL, wrappedJa, wrappedJaCount, 10);
        for (int i = 0; i < wrappedJaCount; i++) {
            M5.Display.drawString(wrappedJa[i], 10, y_pos + i * lineSpacing);
        }

        y_pos += y_spacing + (wrappedJaCount > 1 ? (wrappedJaCount - 1) * lineSpacing : 0);
        

        String wrappedZh[10];
        int wrappedZhCount = 0;
        ::wordWrap(testPhraseZh, maxWidth / testFontSize * FONT_SIZE_ALL, wrappedZh, wrappedZhCount, 10);
        for (int i = 0; i < wrappedZhCount; i++) {
            M5.Display.drawString(wrappedZh[i], 10, y_pos + i * lineSpacing);
        }
        

        M5.Display.setFont(currentFont);
        M5.Display.setTextSize(1.0);
    }
}
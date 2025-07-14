#include "../ui.h"
#include <Arduino.h>
#include <WiFi.h>
#include "../sd_gateway.h"

namespace screens {
    void drawSdGatewayScreen() {
        ::updateHeader();
        RowPosition contentStart = getRowPosition(2);
        RowPosition footerStart = getRowPosition(15);
        M5.Display.fillRect(contentStart.x, contentStart.y, contentStart.width, footerStart.y - contentStart.y, TFT_WHITE);

        String status = sd_gateway::isActive() ? "On port :8080" : "Off";
        ::bufferRow("SD Gateway status: " + status, 3, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true);
        if (sd_gateway::isActive()) {
            ::bufferRow("Веб-интерфейс: http://" + WiFi.localIP().toString() + ":" + String(sd_gateway::getPort()), 6, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true);
            ::bufferRow("Отключите для завершения работы.", 8, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
        } else {
            ::bufferRow("Нажмите SD Gateway на главном экране", 5, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
            ::bufferRow("для запуска сервера.", 6, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
        }
        ::drawRowsBuffered();
        footer.draw(footer.isVisible());
        M5.Display.display();
    }

    void handleSdGatewayAction() {
        // Заглушка для будущих действий
    }
} 
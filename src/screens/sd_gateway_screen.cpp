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
            ::bufferRow("Web interface: http://" + WiFi.localIP().toString() + ":" + String(sd_gateway::getPort()), 6, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, true);
            ::bufferRow("Disconnect to stop the server.", 8, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
        } else {
            ::bufferRow("Press SD Gateway on main screen", 5, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
            ::bufferRow("to start the server.", 6, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
        }
        ::drawRowsBuffered();
        footer.draw(footer.isVisible());
        M5.Display.display();
    }

    void handleSdGatewayAction() {
    }
}
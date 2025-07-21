#include "app_screen.h"
#include "../../ui.h"
#include <vector>

namespace apps_swipe_test {
    
    const int WORK_AREA_X = 0;
    const int WORK_AREA_Y = 170;
    const int WORK_AREA_WIDTH = 540;
    const int WORK_AREA_BOTTOM = 845;
    const int WORK_AREA_HEIGHT = WORK_AREA_BOTTOM - WORK_AREA_Y;
    
    
    static bool showLines = false;
    
    
    const int MIN_SWIPE_DISTANCE = 50;
    const unsigned long MAX_SWIPE_TIME = 1000;
    
    
    struct TouchInfo {
        int startX, startY;
        int currentX, currentY;
        unsigned long startTime;
        bool isActive;
    };
    
    
    struct SwipeRecord {
        String direction;
        unsigned long timestamp;
        int distance;
        int duration;
    };
    
    
    static bool initialized = false;
    static TouchInfo currentTouch = {0, 0, 0, 0, 0, false};
    static std::vector<SwipeRecord> swipeHistory;
    static const int MAX_HISTORY = 10;
    static unsigned long lastUpdateTime = 0;
    static bool needsRedraw = false;
    
    
    int calculateDistance(int x1, int y1, int x2, int y2) {
        int dx = x2 - x1;
        int dy = y2 - y1;
        return sqrt(dx * dx + dy * dy);
    }
    
    
    String determineSwipeDirection(int startX, int startY, int endX, int endY) {
        int dx = endX - startX;
        int dy = endY - startY;
        
    
        int distance = calculateDistance(startX, startY, endX, endY);
        if (distance < MIN_SWIPE_DISTANCE) {
            return "Tap";
        }
        
    
        if (abs(dx) > abs(dy)) {
    
            return (dx > 0) ? "Right" : "Left";
        } else {
    
            return (dy > 0) ? "Down" : "Up";
        }
    }
    
    
    void addSwipeToHistory(const String& direction, int distance, int duration) {
        SwipeRecord record;
        record.direction = direction;
        record.timestamp = millis();
        record.distance = distance;
        record.duration = duration;
        
        swipeHistory.push_back(record);
        
    
        if (swipeHistory.size() > MAX_HISTORY) {
            swipeHistory.erase(swipeHistory.begin());
        }
    }
    
    
    void drawDirectionArrow(const String& direction, int centerX, int centerY, uint16_t color) {
        const int arrowSize = 30;
        
        if (direction == "Up") {
        
            M5.Display.fillTriangle(centerX, centerY - arrowSize, 
                                  centerX - arrowSize/2, centerY + arrowSize/2,
                                  centerX + arrowSize/2, centerY + arrowSize/2, color);
        } else if (direction == "Down") {
        
            M5.Display.fillTriangle(centerX, centerY + arrowSize,
                                  centerX - arrowSize/2, centerY - arrowSize/2,
                                  centerX + arrowSize/2, centerY - arrowSize/2, color);
        } else if (direction == "Left") {
        
            M5.Display.fillTriangle(centerX - arrowSize, centerY,
                                  centerX + arrowSize/2, centerY - arrowSize/2,
                                  centerX + arrowSize/2, centerY + arrowSize/2, color);
        } else if (direction == "Right") {
    
            M5.Display.fillTriangle(centerX + arrowSize, centerY,
                                  centerX - arrowSize/2, centerY - arrowSize/2,
                                  centerX - arrowSize/2, centerY + arrowSize/2, color);
        } else if (direction == "Tap") {
    
            M5.Display.fillCircle(centerX, centerY, arrowSize/2, color);
        }
    }
    
    
    void drawTouchArea() {
    
        if (showLines) {
            M5.Display.drawRect(WORK_AREA_X + 50, WORK_AREA_Y + 50, 
                               WORK_AREA_WIDTH - 100, WORK_AREA_HEIGHT - 100, TFT_BLACK);
        }
        
    
        M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
        M5.Display.setTextDatum(middle_center);
        M5.Display.setTextSize(FONT_SIZE_ALL * 0.8);
        M5.Display.drawString("Touch and swipe here", 
                             WORK_AREA_X + WORK_AREA_WIDTH/2, 
                             WORK_AREA_Y + 100);
        
    
        int centerX = WORK_AREA_X + WORK_AREA_WIDTH/2;
        int centerY = WORK_AREA_Y + WORK_AREA_HEIGHT/2;
        
    
        uint16_t arrowColor = M5.Display.color565(128, 128, 128);
        drawDirectionArrow("Up", centerX, centerY - 80, arrowColor);
        drawDirectionArrow("Down", centerX, centerY + 80, arrowColor);
        drawDirectionArrow("Left", centerX - 80, centerY, arrowColor);
        drawDirectionArrow("Right", centerX + 80, centerY, arrowColor);
    }
    
    
    void drawSwipeHistory() {
        RowPosition footerStart = getRowPosition(15);
        int historyY = footerStart.y - 200;
        
        M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
        M5.Display.setTextDatum(top_left);
        M5.Display.setTextSize(FONT_SIZE_ALL * 0.7);
        

        
    
        int displayCount = min((int)swipeHistory.size(), 5);
        for (int i = 0; i < displayCount; i++) {
            int index = swipeHistory.size() - 1 - i;
            const SwipeRecord& record = swipeHistory[index];
            
            String historyText = String(i + 1) + ". " + record.direction + 
                               " (" + String(record.distance) + "px, " + 
                               String(record.duration) + "ms)";
            
            M5.Display.drawString(historyText, 10, historyY + 30 + i * 25);
        }
    }
    
    
    void drawCurrentState() {
        RowPosition contentStart = getRowPosition(2);
        
        M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
        M5.Display.setTextDatum(top_left);
        M5.Display.setTextSize(FONT_SIZE_ALL);
        
    
        String statusText = String("Touch Status: ") + (currentTouch.isActive ? "Active" : "Inactive");
        M5.Display.drawString(statusText, 10, contentStart.y + 50);
        
    
        if (currentTouch.isActive) {
            String touchText = "Touch: (" + String(currentTouch.currentX) + ", " + 
                             String(currentTouch.currentY) + ")";
            M5.Display.drawString(touchText, 10, contentStart.y + 90);
            
    
            if (calculateDistance(currentTouch.startX, currentTouch.startY,
                                currentTouch.currentX, currentTouch.currentY) > 10) {
                M5.Display.drawLine(currentTouch.startX, currentTouch.startY,
                                  currentTouch.currentX, currentTouch.currentY, TFT_RED);
            }
        }
        

    }
    
    
    void initApp() {
        initialized = true;
        needsRedraw = true;
        lastUpdateTime = millis();
        currentTouch = {0, 0, 0, 0, 0, false};

        swipeHistory.clear();
    }
    
    
void handleTouch(int x, int y, bool isPressed) {
    if (!initialized) {
        initApp();
        return;
    }
    
    unsigned long currentTime = millis();
    
    if (isPressed) {

        if (!currentTouch.isActive) {
            currentTouch.startX = x;
            currentTouch.startY = y;
            currentTouch.currentX = x;
            currentTouch.currentY = y;
            currentTouch.startTime = currentTime;
            currentTouch.isActive = true;
            needsRedraw = true;
        } else {
    
            currentTouch.currentX = x;
            currentTouch.currentY = y;
            needsRedraw = true;
        }
    } else {

        if (currentTouch.isActive) {
            int duration = currentTime - currentTouch.startTime;
            
            if (duration <= MAX_SWIPE_TIME) {
                handleSwipe(currentTouch.startX, currentTouch.startY, x, y);
            }
            
            currentTouch.isActive = false;
            needsRedraw = true;
        }
    }
}
    
    
void handleSwipe(int startX, int startY, int endX, int endY) {
    String direction = determineSwipeDirection(startX, startY, endX, endY);
    int distance = calculateDistance(startX, startY, endX, endY);
    int duration = millis() - currentTouch.startTime;
    

    addSwipeToHistory(direction, distance, duration);
    needsRedraw = true;
}
    
    
    void resetState() {
        swipeHistory.clear();
        currentTouch = {0, 0, 0, 0, 0, false};
        needsRedraw = true;
    }
    

    
    
    void updateAnimation() {
        if (!initialized) {
            initApp();
            return;
        }
        
    
        if (needsRedraw) {
            drawAppScreen();
            needsRedraw = false;
        }
    }
    
    
    void drawAppScreen() {
        if (!initialized) {
            initApp();
        }
        
        ::updateHeader();

    
        RowPosition contentStart = getRowPosition(2);
        RowPosition footerStart = getRowPosition(15);
        M5.Display.fillRect(contentStart.x, contentStart.y, contentStart.width, 
                           footerStart.y - contentStart.y, TFT_WHITE);

    
        ::setUniversalFont();
        
    
        M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
        M5.Display.setTextDatum(top_left);
        M5.Display.setTextSize(FONT_SIZE_ALL);
        M5.Display.drawString("Swipe Test", 10, contentStart.y + 10);
        
    
        drawTouchArea();
        
    
        drawCurrentState();
        
    
        drawSwipeHistory();
    }
}
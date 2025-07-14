#include "app_screen.h"
#include "../../ui.h"
#include <random>

namespace apps_geometry_test {
    // Константы для границ рабочей области
    const int WORK_AREA_X = 0;
    const int WORK_AREA_Y = 170;
    const int WORK_AREA_WIDTH = 540;
    const int WORK_AREA_BOTTOM = 845;
    const int WORK_AREA_HEIGHT = WORK_AREA_BOTTOM - WORK_AREA_Y;
    
    // Переменная для управления отрисовкой горизонтальных линий
    static bool showHorizontalLines = true;
    
    // Структура для хранения параметров фигуры
    struct Shape {
        enum Type { CIRCLE, RECTANGLE, TRIANGLE, LINE } type;
        int x, y, width, height;
        uint16_t color;
        bool filled;
        int strokeWidth;
        float rotation; // Угол поворота в градусах
    };
    
    // Глобальные переменные для анимации
    static unsigned long lastUpdateTime = 0;
    static unsigned long lastTimerUpdate = 0;
    static const unsigned long UPDATE_INTERVAL = 4000; // 4 секунды
    static const unsigned long TIMER_UPDATE_INTERVAL = 1000; // 1 секунда для таймера
    static Shape shapes[10]; // Массив из 10 фигур
    static bool initialized = false;
    static bool needsRedraw = false;
    
    // Генератор случайных чисел
    static std::random_device rd;
    static std::mt19937 gen(rd());
    
    // Функция для генерации оттенков серого (16 уровней для e-ink)
    uint16_t getGrayShade(int level) {
        // level от 0 до 15 (16 уровней серого)
        level = constrain(level, 0, 15);
        int grayValue = (level * 255) / 15;
        return M5.Display.color565(grayValue, grayValue, grayValue);
    }
    
    // Функция для проверки пересечения двух прямоугольников
    bool isOverlapping(const Shape& shape1, const Shape& shape2) {
        // Добавляем небольшой отступ для избежания касания
        int margin = 5;
        
        int x1 = shape1.x - margin;
        int y1 = shape1.y - margin;
        int w1 = shape1.width + 2 * margin;
        int h1 = shape1.height + 2 * margin;
        
        int x2 = shape2.x - margin;
        int y2 = shape2.y - margin;
        int w2 = shape2.width + 2 * margin;
        int h2 = shape2.height + 2 * margin;
        
        return !(x1 >= x2 + w2 || x2 >= x1 + w1 || y1 >= y2 + h2 || y2 >= y1 + h1);
    }
    
    // Функция для проверки пересечения с уже существующими фигурами
    bool hasCollision(const Shape& newShape, const Shape shapes[], int count) {
        for (int i = 0; i < count; i++) {
            if (isOverlapping(newShape, shapes[i])) {
                return true;
            }
        }
        return false;
    }
    
    // Функция для генерации случайной фигуры
    void generateRandomShape(Shape& shape, int contentWidth, int contentHeight, int offsetX, int offsetY) {
        std::uniform_int_distribution<> typeDist(0, 3);
        std::uniform_int_distribution<> colorDist(0, 15);
        std::uniform_int_distribution<> widthDist(10, 180);
        std::uniform_int_distribution<> heightDist(10, 120);
        std::uniform_int_distribution<> strokeDist(1, 8);
        std::uniform_int_distribution<> boolDist(0, 1);
        std::uniform_real_distribution<> rotationDist(0.0, 360.0);
        
        shape.type = static_cast<Shape::Type>(typeDist(gen));
        shape.color = getGrayShade(colorDist(gen));
        shape.filled = boolDist(gen);
        shape.strokeWidth = strokeDist(gen);
        shape.rotation = rotationDist(gen); // Случайный поворот от 0 до 360 градусов
        
        shape.width = widthDist(gen);
        shape.height = heightDist(gen);
        
        // Генерируем позицию с учетом размера фигуры, границ и поворота
        // Добавляем запас для поворота (диагональ прямоугольника)
        int diagonal = sqrt(shape.width * shape.width + shape.height * shape.height);
        int margin = (diagonal - max(shape.width, shape.height)) / 2 + 10;
        
        int maxX = offsetX + contentWidth - shape.width - margin;
        int maxY = offsetY + contentHeight - shape.height - margin;
        int minX = offsetX + margin;
        int minY = offsetY + margin;
        
        // Убеждаемся, что максимальные координаты не меньше минимальных
        if (maxX < minX) maxX = minX;
        if (maxY < minY) maxY = minY;
        
        std::uniform_int_distribution<> xDist(minX, maxX);
        std::uniform_int_distribution<> yDist(minY, maxY);
        
        shape.x = xDist(gen);
        shape.y = yDist(gen);
    }
    
    // Функция для генерации фигуры без пересечений
    void generateNonOverlappingShape(Shape& shape, int contentWidth, int contentHeight, 
                                   int offsetX, int offsetY, const Shape shapes[], int existingCount) {
        const int maxAttempts = 50; // Максимальное количество попыток
        int attempts = 0;
        
        do {
            generateRandomShape(shape, contentWidth, contentHeight, offsetX, offsetY);
            attempts++;
        } while (hasCollision(shape, shapes, existingCount) && attempts < maxAttempts);
        
        // Если не удалось найти место без пересечений, уменьшаем размер
        if (attempts >= maxAttempts && hasCollision(shape, shapes, existingCount)) {
            shape.width = min(shape.width, 30);
            shape.height = min(shape.height, 30);
            
            // Пробуем еще раз с меньшим размером
            attempts = 0;
            do {
                int maxX = offsetX + contentWidth - shape.width;
                int maxY = offsetY + contentHeight - shape.height;
                
                // Убеждаемся, что максимальные координаты не меньше минимальных
                if (maxX < offsetX) maxX = offsetX;
                if (maxY < offsetY) maxY = offsetY;
                
                std::uniform_int_distribution<> xDist(offsetX, maxX);
                std::uniform_int_distribution<> yDist(offsetY, maxY);
                shape.x = xDist(gen);
                shape.y = yDist(gen);
                
                // Дополнительная проверка границ
                if (shape.x + shape.width > offsetX + contentWidth) {
                    shape.x = offsetX + contentWidth - shape.width;
                }
                if (shape.y + shape.height > offsetY + contentHeight) {
                    shape.y = offsetY + contentHeight - shape.height;
                }
                
                attempts++;
            } while (hasCollision(shape, shapes, existingCount) && attempts < maxAttempts);
        }
    }
    
    // Функция для поворота точки относительно центра
    void rotatePoint(int& x, int& y, int centerX, int centerY, float angle) {
        float radians = angle * PI / 180.0;
        float cosA = cos(radians);
        float sinA = sin(radians);
        
        int dx = x - centerX;
        int dy = y - centerY;
        
        x = centerX + (int)(dx * cosA - dy * sinA);
        y = centerY + (int)(dx * sinA + dy * cosA);
    }
    
    // Функция для проверки, находится ли повернутая фигура в границах
    bool isShapeInBounds(const Shape& shape) {
        if (shape.rotation == 0.0) {
            // Простая проверка для неповернутых фигур
            return !(shape.x < WORK_AREA_X || shape.x + shape.width > WORK_AREA_X + WORK_AREA_WIDTH ||
                    shape.y < WORK_AREA_Y || shape.y + shape.height > WORK_AREA_BOTTOM);
        }
        
        // Для повернутых фигур проверяем все углы
        int centerX = shape.x + shape.width / 2;
        int centerY = shape.y + shape.height / 2;
        
        // Проверяем 4 угла прямоугольника
        int corners[4][2] = {
            {shape.x, shape.y},
            {shape.x + shape.width, shape.y},
            {shape.x + shape.width, shape.y + shape.height},
            {shape.x, shape.y + shape.height}
        };
        
        for (int i = 0; i < 4; i++) {
            int x = corners[i][0];
            int y = corners[i][1];
            rotatePoint(x, y, centerX, centerY, shape.rotation);
            
            if (x < WORK_AREA_X || x > WORK_AREA_X + WORK_AREA_WIDTH ||
                y < WORK_AREA_Y || y > WORK_AREA_BOTTOM) {
                return false;
            }
        }
        
        return true;
    }
    
    // Функция для отрисовки фигуры
    void drawShape(const Shape& shape) {
        // Проверяем, что фигура находится внутри границ рабочей области
        if (!isShapeInBounds(shape)) {
            return;
        }
        
        int centerX = shape.x + shape.width / 2;
        int centerY = shape.y + shape.height / 2;
        
        switch (shape.type) {
            case Shape::CIRCLE: {
                // Круги не нуждаются в повороте
                int radius = min(shape.width, shape.height) / 2;
                
                if (shape.filled) {
                    M5.Display.fillCircle(centerX, centerY, radius, shape.color);
                } else {
                    for (int i = 0; i < shape.strokeWidth; i++) {
                        M5.Display.drawCircle(centerX, centerY, radius - i, shape.color);
                    }
                }
                break;
            }
            
            case Shape::RECTANGLE: {
                // Для прямоугольников рисуем 4 линии с поворотом
                int x1 = shape.x, y1 = shape.y;
                int x2 = shape.x + shape.width, y2 = shape.y;
                int x3 = shape.x + shape.width, y3 = shape.y + shape.height;
                int x4 = shape.x, y4 = shape.y + shape.height;
                
                rotatePoint(x1, y1, centerX, centerY, shape.rotation);
                rotatePoint(x2, y2, centerX, centerY, shape.rotation);
                rotatePoint(x3, y3, centerX, centerY, shape.rotation);
                rotatePoint(x4, y4, centerX, centerY, shape.rotation);
                
                if (shape.filled) {
                    // Для заливки используем треугольники
                    M5.Display.fillTriangle(x1, y1, x2, y2, x3, y3, shape.color);
                    M5.Display.fillTriangle(x1, y1, x3, y3, x4, y4, shape.color);
                } else {
                    for (int i = 0; i < shape.strokeWidth; i++) {
                        M5.Display.drawLine(x1, y1, x2, y2, shape.color);
                        M5.Display.drawLine(x2, y2, x3, y3, shape.color);
                        M5.Display.drawLine(x3, y3, x4, y4, shape.color);
                        M5.Display.drawLine(x4, y4, x1, y1, shape.color);
                    }
                }
                break;
            }
            
            case Shape::TRIANGLE: {
                int x1 = shape.x + shape.width / 2;
                int y1 = shape.y;
                int x2 = shape.x;
                int y2 = shape.y + shape.height;
                int x3 = shape.x + shape.width;
                int y3 = shape.y + shape.height;
                
                rotatePoint(x1, y1, centerX, centerY, shape.rotation);
                rotatePoint(x2, y2, centerX, centerY, shape.rotation);
                rotatePoint(x3, y3, centerX, centerY, shape.rotation);
                
                if (shape.filled) {
                    M5.Display.fillTriangle(x1, y1, x2, y2, x3, y3, shape.color);
                } else {
                    for (int i = 0; i < shape.strokeWidth; i++) {
                        M5.Display.drawTriangle(x1, y1, x2, y2, x3, y3, shape.color);
                    }
                }
                break;
            }
            
            case Shape::LINE: {
                int x1 = shape.x;
                int y1 = shape.y;
                int x2 = shape.x + shape.width;
                int y2 = shape.y + shape.height;
                
                rotatePoint(x1, y1, centerX, centerY, shape.rotation);
                rotatePoint(x2, y2, centerX, centerY, shape.rotation);
                
                for (int i = 0; i < shape.strokeWidth; i++) {
                    M5.Display.drawLine(x1 + i, y1 + i, x2 + i, y2 + i, shape.color);
                }
                break;
            }
        }
    }
    
    // Функция для отображения таймера
    void drawTimer(int remainingSeconds, int x, int y) {
        M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
        M5.Display.setTextSize(FONT_SIZE_ALL);
        M5.Display.drawString(String(remainingSeconds) + "s", x, y);
    }
    
    // Функция для обновления только таймера
    void updateTimerOnly() {
        RowPosition footerStart = getRowPosition(15);
        int timerY = footerStart.y - 50;
        int timerX = 10;
        
        // Вычисляем новое время
        unsigned long currentTime = millis();
        unsigned long elapsed = currentTime - lastUpdateTime;
        int remainingSeconds = (UPDATE_INTERVAL - elapsed) / 1000;
        if (remainingSeconds < 0) remainingSeconds = 0;
        
        // Устанавливаем шрифт для вычисления размеров
        ::setUniversalFont();
        M5.Display.setTextSize(FONT_SIZE_ALL);
        
        // Вычисляем точную ширину текста
        String timerText = String(remainingSeconds) + "s";
        int textWidth = M5.Display.textWidth(timerText);
        int textHeight = M5.Display.fontHeight();
        
        // Очищаем только необходимую область с небольшим запасом
        M5.Display.fillRect(timerX, timerY, textWidth + 5, textHeight + 2, TFT_WHITE);
        
        // Отображаем новое время
        drawTimer(remainingSeconds, timerX, timerY);
    }
    
    // Функция инициализации приложения
    void initApp() {
        unsigned long currentTime = millis();
        lastUpdateTime = currentTime;
        lastTimerUpdate = currentTime;
        initialized = true;
        needsRedraw = true;
        
        // Генерируем начальные фигуры
        RowPosition contentStart = getRowPosition(2);
        RowPosition footerStart = getRowPosition(15);
        
        // Генерируем фигуры в рабочей области
        for (int i = 0; i < 10; i++) {
            generateNonOverlappingShape(shapes[i], WORK_AREA_WIDTH, WORK_AREA_HEIGHT, 
                                      WORK_AREA_X, WORK_AREA_Y, shapes, i);
        }
    }
    
    // Функция для обновления анимации
    void updateAnimation() {
        if (!initialized) {
            initApp();
            return;
        }
        
        unsigned long currentTime = millis();
        bool shouldRedraw = false;
        
        // Проверяем, нужно ли обновить фигуры
        if (currentTime - lastUpdateTime >= UPDATE_INTERVAL) {
            lastUpdateTime = currentTime;
            lastTimerUpdate = currentTime; // Синхронизируем таймер
            
            // Генерируем новые фигуры
            RowPosition contentStart = getRowPosition(2);
            RowPosition footerStart = getRowPosition(15);
            
            // Генерируем новые фигуры в рабочей области
            for (int i = 0; i < 10; i++) {
                generateNonOverlappingShape(shapes[i], WORK_AREA_WIDTH, WORK_AREA_HEIGHT, 
                                          WORK_AREA_X, WORK_AREA_Y, shapes, i);
            }
            
            shouldRedraw = true;
        }
        // Проверяем, нужно ли обновить только таймер
        else if (currentTime - lastTimerUpdate >= TIMER_UPDATE_INTERVAL) {
            lastTimerUpdate = currentTime;
            updateTimerOnly(); // Обновляем только таймер
            return;
        }
        
        // Перерисовываем экран только если нужно
        if (shouldRedraw || needsRedraw) {
            drawAppScreen();
            needsRedraw = false;
        }
    }
    
    // Функция для отображения экрана приложения
    void drawAppScreen() {
        if (!initialized) {
            initApp();
        }
        
        ::updateHeader(); // Обновляем заголовок для этого экрана

        // Очищаем область контента
        RowPosition contentStart = getRowPosition(2);
        RowPosition footerStart = getRowPosition(15);
        M5.Display.fillRect(contentStart.x, contentStart.y, contentStart.width, 
                           footerStart.y - contentStart.y, TFT_WHITE);

        // Устанавливаем универсальный шрифт
        ::setUniversalFont();
        
        // Отображаем заголовок приложения
        M5.Display.setTextColor(TFT_BLACK, TFT_WHITE);
        M5.Display.setTextDatum(top_left);
        M5.Display.setTextSize(FONT_SIZE_ALL);
        M5.Display.drawString("Geometry Test - Random Shapes", 10, contentStart.y + 10);
        
        // Рисуем все фигуры
        for (int i = 0; i < 10; i++) {
            drawShape(shapes[i]);
        }
        
        // Рисуем горизонтальные линии по границам рабочей области (если включено)
        if (showHorizontalLines) {
            // Верхняя горизонтальная граница (по всей ширине экрана)
            M5.Display.drawLine(WORK_AREA_X, WORK_AREA_Y, WORK_AREA_X + WORK_AREA_WIDTH - 1, WORK_AREA_Y, TFT_BLACK);
            
            // Нижняя горизонтальная граница (по всей ширине экрана)
            M5.Display.drawLine(WORK_AREA_X, WORK_AREA_BOTTOM, WORK_AREA_X + WORK_AREA_WIDTH - 1, WORK_AREA_BOTTOM, TFT_BLACK);
        }
        
        // Отображаем таймер
        unsigned long currentTime = millis();
        unsigned long elapsed = currentTime - lastUpdateTime;
        int remainingSeconds = (UPDATE_INTERVAL - elapsed) / 1000;
        if (remainingSeconds < 0) remainingSeconds = 0;
        
        drawTimer(remainingSeconds, 10, footerStart.y - 50);
    }
    
    // Функция для переключения отображения горизонтальных линий
    void toggleHorizontalLines() {
        showHorizontalLines = !showHorizontalLines;
        needsRedraw = true; // Принудительная перерисовка
    }
    
    // Функция для получения состояния отображения горизонтальных линий
    bool getHorizontalLinesState() {
        return showHorizontalLines;
    }
}
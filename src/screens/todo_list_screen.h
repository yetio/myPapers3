#ifndef TODO_LIST_SCREEN_H
#define TODO_LIST_SCREEN_H

#include <Arduino.h>

namespace apps_todo {
    void drawTodoListScreen();
    void handleTodoListTouch(int row, int x, int y);
}

namespace screens {
    void drawTodoListScreen();
    void handleTodoListTouch(int row, int x, int y);
    // pagination callbacks used by footer
    void todoPrevPage();
    void todoNextPage();
    void todoBackHome();
}

#endif

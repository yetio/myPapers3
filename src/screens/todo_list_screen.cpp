#include "todo_list_screen.h"
#include "../ui.h"
#include "../storage/tasks.h"
#include <Arduino.h>


using namespace screens;

static const size_t PAGE_SIZE = 12; // rows 2..13
static size_t g_page = 0;

// forward declarations
void todoPrevPage();
void todoNextPage();
void todoBackHome();

void screens::drawTodoListScreen() {
    Task tasks[PAGE_SIZE];
    size_t totalCount = 0;
    // Load all tasks into a larger buffer to count total; for now reuse loader paged
    // We'll load starting at offset = g_page * PAGE_SIZE by loading all and slicing
    // For simplicity, load large max (e.g., 200)
    const size_t MAX_ALL = 200;
    Task allTasks[MAX_ALL];
    size_t loaded = 0;
    bool okAll = tasks_load_structured(allTasks, MAX_ALL, loaded);
    if (!okAll || loaded == 0) {
        ::bufferRow("No tasks", 2, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
        for (int r = 3; r <= 13; ++r) ::bufferRow("", r, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
        return;
    }

    totalCount = loaded;
    size_t start = g_page * PAGE_SIZE;
    if (start >= totalCount) {
        // clamp
        g_page = 0;
        start = 0;
    }

    size_t toShow = min(PAGE_SIZE, totalCount - start);
    int row = 2;
    for (size_t i = 0; i < toShow && row <= 13; ++i, ++row) {
        Task &t = allTasks[start + i];
        String line = (t.completed ? "[x] " : "[ ] ") + t.title;
        ::bufferRow(line, row, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);
    }

    for (int r = row; r <= 13; ++r) ::bufferRow("", r, TFT_BLACK, TFT_WHITE, FONT_SIZE_ALL, false);

    // footer buttons (Prev, Home, Next) handled via ui.cpp footer assignment
}

void screens::handleTodoListTouch(int row, int x, int y) {
    // Determine which index in page was touched
    if (row < 2 || row > 13) return;
    int pageIndex = row - 2;

    const size_t MAX_ALL = 200;
    Task allTasks[MAX_ALL];
    size_t loaded = 0;
    if (!tasks_load_structured(allTasks, MAX_ALL, loaded)) return;
    size_t start = g_page * PAGE_SIZE;
    size_t idx = start + pageIndex;
    if (idx >= loaded) return;

    // toggle completed and save entire list
    allTasks[idx].completed = !allTasks[idx].completed;
    tasks_save_structured(allTasks, loaded);

    renderCurrentScreen();
}

void screens::todoPrevPage() {
    if (g_page == 0) return;
    g_page--;
    renderCurrentScreen();
}

void screens::todoNextPage() {
    const size_t MAX_ALL = 200;
    Task allTasks[MAX_ALL];
    size_t loaded = 0;
    if (!tasks_load_structured(allTasks, MAX_ALL, loaded)) return;
    size_t pages = (loaded + PAGE_SIZE - 1) / PAGE_SIZE;
    if (g_page + 1 < pages) {
        g_page++;
        renderCurrentScreen();
    }
}

void screens::todoBackHome() {
    currentScreen = MAIN_SCREEN;
    renderCurrentScreen();
}

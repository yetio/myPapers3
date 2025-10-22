#ifndef TASKS_H
#define TASKS_H

#include <Arduino.h>

struct Task {
    uint32_t id;
    String title;
    String notes;
    uint32_t due_date; // epoch seconds
    bool completed;
    uint8_t priority;
    uint32_t created_at;
    uint32_t updated_at;
};

// Initialize storage (create /tasks directory if needed)
bool tasks_init();

// Load entire tasks file into provided JSON string (raw).
// Returns true on success.
bool tasks_load_all(String &outJson, const char* path = "/tasks/todo.json");

// Save JSON string to file (overwrites). Returns true on success.
bool tasks_save_all(const String &jsonData, const char* path = "/tasks/todo.json");

// Simple helpers: append a task represented as JSON object string to existing list (not efficient)
bool tasks_add_raw(const String &taskJson, const char* path = "/tasks/todo.json");

// Delete file (e.g., for tests)
bool tasks_delete_file(const char* path = "/tasks/todo.json");

// Convert Todoist REST /tasks JSON (array) into local task JSON array string.
// Returns true on success and writes resulting JSON array to outJson.
bool tasks_convert_from_todoist(const String &todoistJson, String &outJson);

// Structured read/write helpers
// Load up to maxTasks into outTasks, set outCount to number loaded. Returns true on success.
bool tasks_load_structured(Task *outTasks, size_t maxTasks, size_t &outCount, const char* path = "/tasks/todo.json");

// Save an array of Task structs to path (overwrites). Returns true on success.
bool tasks_save_structured(const Task *tasks, size_t count, const char* path = "/tasks/todo.json");

#endif

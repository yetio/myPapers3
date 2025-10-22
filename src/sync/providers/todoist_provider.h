#ifndef TODOIST_PROVIDER_H
#define TODOIST_PROVIDER_H

#include <Arduino.h>

class TodoistProvider {
public:
    TodoistProvider();
    void begin(const String &apiToken);
    bool isAuthenticated();
    // Pull tasks from Todoist (simple list), returns JSON string or empty on failure
    String pull_tasks();
    // Push a task JSON to Todoist, returns remote id or empty on failure
    String push_task(const String &taskJson);
    // Delete remote task by id
    bool delete_task(const String &remoteId);
private:
    String token;
};

#endif

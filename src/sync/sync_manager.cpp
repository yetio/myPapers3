#include "sync_manager.h"
#include <SD.h>
#include "settings.h"
#include "sync/providers/todoist_provider.h"
#include "storage/tasks.h"
#include "network/wifi_manager.h"

// pull interval in milliseconds (5 minutes)
static const unsigned long PULL_INTERVAL_MS = 5UL * 60UL * 1000UL;
static unsigned long g_last_pull = 0;
static TodoistProvider g_todoist;

static SyncState g_state = SYNC_IDLE;
static String g_last_error = "";

void sync_manager_init() {
    g_state = SYNC_IDLE;
    g_last_error = "";
    g_last_pull = 0;
}

void sync_manager_loop() {
    // Basic pull-only sync flow:
    // - if WiFi connected and we have a Todoist token, periodically pull tasks
    // - save returned JSON to /tasks/todo.json using tasks_save_all

    g_state = SYNC_IDLE;

    // check wifi
    if (WiFi.status() != WL_CONNECTED) {
        // nothing to do
        return;
    }

    // check settings for token
    Settings settings;
    String token = settings.getTodoistToken();
    if (token.length() == 0) {
        // no token configured
        return;
    }

    unsigned long now = millis();
    if (g_last_pull != 0 && (now - g_last_pull) < PULL_INTERVAL_MS) {
        return; // not time yet
    }

    // perform pull
    g_state = SYNC_RUNNING;
    g_todoist.begin(token);
    String body = g_todoist.pull_tasks();
    if (body.length() == 0) {
        g_state = SYNC_ERROR;
        g_last_error = "Todoist pull failed or returned empty";
        g_last_pull = now; // avoid busy loops
        return;
    }

    // ensure tasks dir exists
    tasks_init();
    String localJson;
    bool converted = tasks_convert_from_todoist(body, localJson);
    bool ok = false;
    if (converted) {
        ok = tasks_save_all(localJson, "/tasks/todo.json");
    } else {
        // fallback: save raw body
        ok = tasks_save_all(body, "/tasks/todo.json");
    }
    if (!ok) {
        g_state = SYNC_ERROR;
        g_last_error = "Failed to save tasks to SD";
        g_last_pull = now;
        return;
    }

    g_state = SYNC_IDLE;
    g_last_error = "";
    g_last_pull = now;
}

SyncState sync_manager_get_state() {
    return g_state;
}

String sync_manager_get_last_error() {
    return g_last_error;
}

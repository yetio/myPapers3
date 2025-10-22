#include "todoist_provider.h"
#include <HTTPClient.h>

TodoistProvider::TodoistProvider() {}

void TodoistProvider::begin(const String &apiToken) {
    token = apiToken;
}

bool TodoistProvider::isAuthenticated() {
    return token.length() > 0;
}

String TodoistProvider::pull_tasks() {
    if (!isAuthenticated()) return String("");
    HTTPClient http;
    http.begin("https://api.todoist.com/rest/v1/tasks");
    http.addHeader("Authorization", String("Bearer ") + token);
    int code = http.GET();
    if (code != 200) {
        http.end();
        return String("");
    }
    String body = http.getString();
    http.end();
    return body;
}

String TodoistProvider::push_task(const String &taskJson) {
    if (!isAuthenticated()) return String("");
    HTTPClient http;
    http.begin("https://api.todoist.com/rest/v1/tasks");
    http.addHeader("Authorization", String("Bearer ") + token);
    http.addHeader("Content-Type", "application/json");
    int code = http.POST(taskJson);
    if (code != 200 && code != 204 && code != 201) {
        http.end();
        return String("");
    }
    String body = http.getString();
    http.end();
    // Caller should parse returned JSON to extract id
    return body;
}

bool TodoistProvider::delete_task(const String &remoteId) {
    if (!isAuthenticated() || remoteId.length() == 0) return false;
    HTTPClient http;
    String url = String("https://api.todoist.com/rest/v1/tasks/") + remoteId;
    http.begin(url);
    http.addHeader("Authorization", String("Bearer ") + token);
    int code = http.sendRequest("DELETE");
    http.end();
    return (code == 204 || code == 200);
}

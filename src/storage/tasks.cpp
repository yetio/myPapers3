#include "tasks.h"
#include <SD.h>
#include <ArduinoJson.h>
#include <time.h>

bool tasks_init() {
    if (!SD.exists("/tasks")) {
        return SD.mkdir("/tasks");
    }
    return true;
}

bool tasks_load_all(String &outJson, const char* path) {
    if (!SD.exists(path)) return false;
    File f = SD.open(path, FILE_READ);
    if (!f) return false;
    outJson = "";
    while (f.available()) {
        outJson += (char)f.read();
    }
    f.close();
    return true;
}

bool tasks_save_all(const String &jsonData, const char* path) {
    // write to tmp then rename
    String tmp = String(path) + ".tmp";
    File f = SD.open(tmp.c_str(), FILE_WRITE);
    if (!f) return false;
    size_t written = f.print(jsonData);
    f.close();
    if (written == 0) return false;
    // remove original and rename
    if (SD.exists(path)) SD.remove(path);
    return SD.rename(tmp.c_str(), path);
}

bool tasks_add_raw(const String &taskJson, const char* path) {
    String all;
    if (!tasks_load_all(all, path)) {
        // create new array
        String arr = "[" + taskJson + "]";
        return tasks_save_all(arr, path);
    }
    // naive append before the last ]
    if (all.endsWith("]")) {
        all.remove(all.length()-1);
        if (all.length() > 1) all += ",";
        all += taskJson;
        all += "]";
        return tasks_save_all(all, path);
    }
    return false;
}

bool tasks_delete_file(const char* path) {
    if (SD.exists(path)) return SD.remove(path);
    return true;
}

bool tasks_convert_from_todoist(const String &todoistJson, String &outJson) {
    // parse incoming array
    if (todoistJson.length() == 0) return false;
    // estimate size: each task ~512 bytes, multiply by count heuristic
    const size_t CAP = 2048;
    DynamicJsonDocument inDoc(CAP);
    DeserializationError err = deserializeJson(inDoc, todoistJson);
    if (err) return false;

    // input should be an array
    if (!inDoc.is<JsonArray>()) {
        // if it's an object, try to wrap single
        // we'll still attempt to treat as array
    }

    JsonArray arr = inDoc.as<JsonArray>();

    // build output array
    DynamicJsonDocument outDoc(CAP * 2);
    JsonArray outArr = outDoc.to<JsonArray>();

    for (JsonVariant v : arr) {
        JsonObject obj = v.as<JsonObject>();
        JsonObject t = outArr.createNestedObject();
        // remote id
        if (obj.containsKey("id")) t["remote_id"] = obj["id"].as<String>();
        // content / title
        if (obj.containsKey("content")) t["title"] = obj["content"].as<String>();
        else if (obj.containsKey("title")) t["title"] = obj["title"].as<String>();
        else t["title"] = "";
        // notes / description
        if (obj.containsKey("description")) t["notes"] = obj["description"].as<String>();
        else if (obj.containsKey("notes")) t["notes"] = obj["notes"].as<String>();
        else t["notes"] = "";

        // due: Todoist may provide due.date or due.datetime
        uint32_t due_ts = 0;
        if (obj.containsKey("due") && obj["due"].is<JsonObject>()) {
            JsonObject due = obj["due"].as<JsonObject>();
            if (due.containsKey("date")) {
                String datestr = due["date"].as<String>();
                // try parse YYYY-MM-DD or ISO
                struct tm tm{};
                if (strptime(datestr.c_str(), "%Y-%m-%dT%H:%M:%S", &tm) != NULL) {
                    due_ts = mktime(&tm);
                } else if (strptime(datestr.c_str(), "%Y-%m-%d", &tm) != NULL) {
                    due_ts = mktime(&tm);
                }
            } else if (due.containsKey("datetime")) {
                String datestr = due["datetime"].as<String>();
                struct tm tm{};
                if (strptime(datestr.c_str(), "%Y-%m-%dT%H:%M:%S", &tm) != NULL) {
                    due_ts = mktime(&tm);
                }
            }
        }
        t["due_date"] = due_ts;

        // completed
        if (obj.containsKey("is_completed")) t["completed"] = obj["is_completed"].as<bool>();
        else if (obj.containsKey("checked")) t["completed"] = obj["checked"].as<bool>();
        else t["completed"] = false;

        // priority (Todoist: 1..4) -> store as is
        if (obj.containsKey("priority")) t["priority"] = obj["priority"].as<int>();
        else t["priority"] = 0;

        // created/updated timestamps if present
        if (obj.containsKey("created_at")) t["created_at"] = 0; // placeholder
        if (obj.containsKey("added_at")) t["created_at"] = 0;
    }

    // serialize outDoc to string
    String out;
    if (serializeJson(outArr, out) == 0) return false;
    outJson = out;
    return true;
}

bool tasks_load_structured(Task *outTasks, size_t maxTasks, size_t &outCount, const char* path) {
    outCount = 0;
    String raw;
    if (!tasks_load_all(raw, path)) return false;
    if (raw.length() == 0) return false;

    const size_t CAP = 2048;
    DynamicJsonDocument doc(CAP);
    DeserializationError err = deserializeJson(doc, raw);
    if (err) return false;

    JsonArray arr = doc.as<JsonArray>();
    size_t idx = 0;
    for (JsonVariant v : arr) {
        if (idx >= maxTasks) break;
        JsonObject o = v.as<JsonObject>();
        Task &t = outTasks[idx];
        t.id = idx + 1; // local id assignment
        if (o.containsKey("title")) t.title = o["title"].as<String>(); else t.title = "";
        if (o.containsKey("notes")) t.notes = o["notes"].as<String>(); else t.notes = "";
        if (o.containsKey("due_date")) t.due_date = o["due_date"].as<uint32_t>(); else t.due_date = 0;
        if (o.containsKey("completed")) t.completed = o["completed"].as<bool>(); else t.completed = false;
        if (o.containsKey("priority")) t.priority = o["priority"].as<uint8_t>(); else t.priority = 0;
        if (o.containsKey("created_at")) t.created_at = o["created_at"].as<uint32_t>(); else t.created_at = 0;
        if (o.containsKey("updated_at")) t.updated_at = o["updated_at"].as<uint32_t>(); else t.updated_at = 0;
        idx++;
    }
    outCount = idx;
    return true;
}

bool tasks_save_structured(const Task *tasks, size_t count, const char* path) {
    const size_t CAP = 1024 + (count * 256);
    DynamicJsonDocument doc(CAP);
    JsonArray arr = doc.to<JsonArray>();
    for (size_t i = 0; i < count; ++i) {
        const Task &t = tasks[i];
        JsonObject o = arr.createNestedObject();
        o["title"] = t.title;
        o["notes"] = t.notes;
        o["due_date"] = t.due_date;
        o["completed"] = t.completed;
        o["priority"] = t.priority;
        o["created_at"] = t.created_at;
        o["updated_at"] = t.updated_at;
        // remote_id isn't part of Task struct currently; could be extended
    }

    String out;
    if (serializeJson(arr, out) == 0) return false;
    return tasks_save_all(out, path);
}

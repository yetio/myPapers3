#include <Arduino.h>
#include <SD.h>
#include <WiFi.h>
#include <WebServer.h>
#include "sd_gateway.h"
#include "debug_config.h"
#include "ui.h"

namespace sd_gateway {
    static bool active = false;
    static uint16_t serverPort = 8080;
    static WebServer* server = nullptr;

    bool isActive() { return active; }
    uint16_t getPort() { return serverPort; }

    void handleRoot() {
        String html = "<html><head><title>SD Gateway</title></head><body>";
        html += "<h2>SD Gateway</h2>";
        html += "<form method='POST' action='/upload' enctype='multipart/form-data'>";
        html += "<input type='file' name='file'><input type='submit' value='Upload'></form>";
        html += "<h3>Files:</h3>";
        html += "<form method='POST' action='/delete_multi' onsubmit='return confirm(\"Delete selected files?\");'>";
        html += "<ul>";
        File root = SD.open("/");
        while (true) {
            File entry = root.openNextFile();
            if (!entry) break;
            String name = entry.name();
            html += "<li>";
            html += "<input type='checkbox' name='file' value='" + name + "'> ";
            html += name + " <a href='/delete?file=" + name + "'>[delete]</a>";
            if (name.endsWith(".txt")) {
                html += " <a href='/edit?file=" + name + "'>[edit]</a>";
            }
            html += "</li>";
            entry.close();
        }
        root.close();
        html += "</ul>";
        html += "<input type='submit' value='Delete selected'>";
        html += "</form>";
        html += "</body></html>";
        server->send(200, "text/html", html);
    }

    void handleUpload() {
        HTTPUpload& upload = server->upload();
        static File uploadFile;
        if (upload.status == UPLOAD_FILE_START) {
            String filename = "/" + upload.filename;
            uploadFile = SD.open(filename, FILE_WRITE);
        } else if (upload.status == UPLOAD_FILE_WRITE) {
            if (uploadFile) uploadFile.write(upload.buf, upload.currentSize);
        } else if (upload.status == UPLOAD_FILE_END) {
            if (uploadFile) uploadFile.close();
            server->sendHeader("Location", "/");
            server->send(303);
        }
    }

    void handleDelete() {
        if (!server->hasArg("file")) {
            server->send(400, "text/plain", "Missing file param");
            return;
        }
        String filename = server->arg("file");
        if (!filename.startsWith("/")) filename = "/" + filename;
        #ifdef DEBUG_SD_GATEWAY
        Serial.print("[SD Gateway] Delete request for: ");
        Serial.println(filename);
        #endif
        if (SD.exists(filename)) {
            SD.remove(filename);
            server->sendHeader("Location", "/");
            server->send(303);
        } else {
            server->send(404, "text/plain", "File not found: " + filename);
        }
    }

    void handleDeleteMulti() {
        if (!server->hasArg("file")) {
            server->sendHeader("Location", "/");
            server->send(303);
            return;
        }
        int n = server->args();
        for (int i = 0; i < n; ++i) {
            if (server->argName(i) == "file") {
                String filename = server->arg(i);
                if (!filename.startsWith("/")) filename = "/" + filename;
                #ifdef DEBUG_SD_GATEWAY
                Serial.print("[SD Gateway] Multi-delete: ");
                Serial.println(filename);
                #endif
                if (SD.exists(filename)) {
                    SD.remove(filename);
                }
            }
        }
        server->sendHeader("Location", "/");
        server->send(303);
    }

    void handleEditGet() {
        if (!server->hasArg("file")) {
            server->send(400, "text/plain", "Missing file param");
            return;
        }
        String filename = server->arg("file");
        if (!filename.startsWith("/")) filename = "/" + filename;
        if (!filename.endsWith(".txt")) {
            server->send(403, "text/plain", "Editing only allowed for .txt files");
            return;
        }
        File file = SD.open(filename, FILE_READ);
        if (!file) {
            server->send(404, "text/plain", "File not found");
            return;
        }
        String content;
        while (file.available()) {
            content += (char)file.read();
        }
        file.close();
        String html = "<html><head><title>Edit " + filename + "</title></head><body>";
        html += "<h2>Edit: " + filename + "</h2>";
        html += "<form method='POST' action='/edit'>";
        html += "<input type='hidden' name='file' value='" + filename + "'>";
        html += "<textarea name='content' rows='25' cols='80'>";
        // Экранируем спецсимволы для HTML
        for (size_t i = 0; i < content.length(); ++i) {
            char c = content[i];
            if (c == '<') html += "&lt;";
            else if (c == '>') html += "&gt;";
            else if (c == '&') html += "&amp;";
            else html += c;
        }
        html += "</textarea><br>";
        html += "<input type='submit' value='Save'> ";
        html += "<a href='/'>Cancel</a>";
        html += "</form></body></html>";
        server->send(200, "text/html", html);
    }

    void handleEditPost() {
        if (!server->hasArg("file") || !server->hasArg("content")) {
            server->send(400, "text/plain", "Missing file or content param");
            return;
        }
        String filename = server->arg("file");
        if (!filename.startsWith("/")) filename = "/" + filename;
        String content = server->arg("content");
        File file = SD.open(filename, FILE_WRITE);
        if (!file) {
            server->send(500, "text/plain", "Failed to open file for writing");
            return;
        }
        file.print(content);
        file.close();
        server->sendHeader("Location", "/");
        server->send(303);
    }

    void handleList() {
        String json = "[";
        File root = SD.open("/");
        bool first = true;
        while (true) {
            File entry = root.openNextFile();
            if (!entry) break;
            if (!first) json += ",";
            json += "\"" + String(entry.name()) + "\"";
            first = false;
            entry.close();
        }
        root.close();
        json += "]";
        server->send(200, "application/json", json);
    }

    void startServer() {
        if (active) return;
        if (!SD.begin()) {
            displayMessage("SD init error");
            return;
        }
        server = new WebServer(serverPort);
        server->on("/", HTTP_GET, handleRoot);
        server->on("/upload", HTTP_POST, [](){ server->send(200); }, handleUpload);
        server->on("/delete", HTTP_GET, handleDelete);
        server->on("/delete_multi", HTTP_POST, handleDeleteMulti);
        server->on("/edit", HTTP_GET, handleEditGet);
        server->on("/edit", HTTP_POST, handleEditPost);
        server->on("/list", HTTP_GET, handleList);
        server->begin();
        active = true;
    }

    void stopServer() {
        if (server) {
            server->close();
            delete server;
            server = nullptr;
        }
        active = false;
    }

    void toggleOrShow() {
        if (WiFi.status() != WL_CONNECTED) {
            displayMessage("Wi-Fi не подключен");
            return;
        }
        if (active) {
            stopServer();
            displayMessage("SD Gateway: Off");
        } else {
            startServer();
            displayMessage("SD Gateway: On (port " + String(serverPort) + ")");
        }
    }

    void loop() {
        if (active && server) {
            server->handleClient();
        }
    }
}
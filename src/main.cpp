#include "chain.h"
#include "secrets.h"
#include <Arduino.h>
#include <ArduinoJson.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <FS.h> // Include the SPIFFS library
#include <JeVe_EasyOTA.h>
#include <WiFiClient.h>

ESP8266WebServer server(80); // Create a webserver object that listens for HTTP request on port 80
EasyOTA OTA(hostname);

String getContentType(const String& filename); // convert the file extension to the MIME type
bool handleAction(const String& path);
bool handleFileRead(String path); // send the right file to the client (if it exists)

time_t startTime;

Chain chain(15);

void setupWiFi()
{
    WiFi.begin(ssid, password);
    WiFi.hostname(hostname);

    Serial.print("Connecting ");
    while (WiFi.status() != WL_CONNECTED) {
        chain.loop();
        yield();
        Serial.print(".");
    }

    Serial.print("Connected\n IP address: ");
    Serial.print(WiFi.localIP().toString());
    Serial.print("\n Hostname: ");
    Serial.print(WiFi.hostname());

    server.onNotFound([]() { // If the client requests any URI
        if (!handleAction(server.uri())
            && !handleFileRead(server.uri())) { // send it if it exists
            server.send(404, "text/plain", "404: Not Found"); // otherwise, respond with a 404 (Not Found) error
        }
    });

    server.begin(); // Actually start the server
    Serial.print("\nServer running!");

    OTA.addAP(ssid, password);
    Serial.print("\nOTA active!");
}

tm* getLocalTime()
{
    time_t t(startTime + (millis() / 1000));
    return localtime(&t);
}

void parseJSON(const char* j)
{
    const size_t capacity = JSON_OBJECT_SIZE(15) + 400;
    DynamicJsonDocument doc(capacity);
    deserializeJson(doc, j);
    auto unixtime = doc["unixtime"].as<time_t>();
    auto raw_offset = doc["raw_offset"].as<time_t>();
    auto dst_offset = doc["dst_offset"].as<time_t>();
    startTime = unixtime + raw_offset + dst_offset - (millis() / 1000);
}

void getTime()
{
    WiFiClient client;
    HTTPClient http;
    Serial.print("[HTTP] begin...\n");
    if (http.begin(client, "http://worldtimeapi.org/api/ip")) { // HTTP
        Serial.print("[HTTP] GET...\n");
        // start connection and send HTTP header
        int httpCode = http.GET();
        // httpCode will be negative on error
        if (httpCode > 0) {
            // HTTP header has been send and Server response header has been handled
            Serial.printf("[HTTP] GET... code: %d\n", httpCode);
            // file found at server
            if (httpCode == HTTP_CODE_OK || httpCode == HTTP_CODE_MOVED_PERMANENTLY) {
                String payload = http.getString();
                Serial.println(payload);
                parseJSON(payload.c_str());
            }
        } else {
            Serial.printf("[HTTP] GET... failed, error: %s\n", http.errorToString(httpCode).c_str());
        }
        http.end();
    } else {
        Serial.printf("[HTTP} Unable to connect\n");
    }
}

void setup()
{
    Serial.begin(115200);
    Serial.printf("Go! \n");

    chain.begin();
    setupWiFi();
    getTime();
    SPIFFS.begin(); // Start the SPI Flash Files System
    chain.first();
}

uint32_t lastTime(millis());
void loop()
{
    server.handleClient();
    OTA.loop();
    chain.loop();

    if (millis() - lastTime > 60000) {
        lastTime = millis();
        tm* t(getLocalTime());
        Serial.printf("%u.%u.%u %02u:%02u:%02u\n",
            t->tm_mday, t->tm_mon + 1, t->tm_year + 1900,
            t->tm_hour, t->tm_min, t->tm_sec);
        if (t->tm_hour >= 22) {
            chain.off();
        }
    }
}

bool handleAction(const String& path)
{
    if (path.startsWith("/action_")) {
        uint8_t id = path.substring(8).toInt();
        Serial.printf("handleAction: %u\n", id);

        chain.action(id);

        server.send(200, "text/plain", "OK '" + path + "'");
        return true;
    }
    return false;
}

String getContentType(const String& filename)
{
    if (filename.endsWith(".html")) {
        return "text/html";
    } else if (filename.endsWith(".css")) {
        return "text/css";
    } else if (filename.endsWith(".js")) {
        return "application/javascript";
    } else if (filename.endsWith(".ico")) {
        return "image/x-icon";
    }
    return "text/plain";
}

bool handleFileRead(String path)
{
    Serial.printf("handleFileRead: %s\n", path.c_str());
    if (path.endsWith("/")) {
        path += "index.html"; // If a folder is requested, send the index file
    }
    String contentType = getContentType(path); // Get the MIME type
    if (SPIFFS.exists(path)) { // If the file exists
        File file = SPIFFS.open(path, "r"); // Open it
        size_t sent = server.streamFile(file, contentType); // And send it to the client
        file.close(); // Then close the file again
        Serial.printf("\tSent %u bytes\n", sent);
        return true;
    }
    Serial.print("\tFile Not Found\n");
    return false; // If the file doesn't exist, return false
}

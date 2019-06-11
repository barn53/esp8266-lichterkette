#include "chain.h"
#include "secrets.h"
#include <Arduino.h>
#include <ESP8266WebServer.h>
#include <ESP8266WiFi.h>
#include <FS.h> // Include the SPIFFS library
#include <JeVe_EasyOTA.h>

ESP8266WebServer server(80); // Create a webserver object that listens for HTTP request on port 80
EasyOTA OTA(hostname);

String getContentType(const String& filename); // convert the file extension to the MIME type
bool handleAction(const String& path);
bool handleFileRead(String path); // send the right file to the client (if it exists)

Chain chain(20);

void setupWiFi()
{
    WiFi.begin(ssid, password);
    WiFi.hostname(hostname);

    Serial.print("Connecting ");
    while (WiFi.status() != WL_CONNECTED) {
        delay(100);
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

void setup()
{
    Serial.begin(115200);
    Serial.printf("Go! \n");
    setupWiFi();

    SPIFFS.begin(); // Start the SPI Flash Files System

    chain.begin();
}

void loop()
{
    server.handleClient();
    OTA.loop();
    chain.loop();
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

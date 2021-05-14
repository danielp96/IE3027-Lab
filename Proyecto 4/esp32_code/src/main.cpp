
// call with web consoloe this, must be in / not in /json
// fetch("http://192.168.0.26/json?led1=1&led2=0&led3=0&led4=1", {method: "GET"}).then(res => res.json()).then(console.log)
//

// do not use delay, interferes with async
// use millis() instead

#include <Arduino.h>

#include <WiFiClient.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"

const char* ssid = "TigoStar_Pineda";
const char* password = "Pineda.Tigo.Wifi";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const int led = 13;
int led_status = 1;

String index_html;
String index_js  ;
String index_css ;

void handleRoot(AsyncWebServerRequest* request);
void handleNotFound(AsyncWebServerRequest* request);
void handleJson(AsyncWebServerRequest* request);
void handleJs(AsyncWebServerRequest* request);
void init_html(void);
void init_js(void);
void handleWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len);

void setup(void)
{
    pinMode(led, OUTPUT);
    digitalWrite(led, 0);

    Serial.begin(115200);

    Serial.println("");

    // wifi config
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(500);
        Serial.print(".");
    }
    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(ssid);
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());

    init_html();
    init_js();

    ws.onEvent(handleWsEvent);
    server.addHandler(&ws);

    server.on("/", handleRoot);
    server.on("/index.js", handleJs);

    server.on("/json", handleJson);

    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP server started");
}

void loop(void)
{
    if (millis() % 10000 == 500)
    {
        ws.textAll("{\"led1\":\"1\",\"led2\":\"0\",\"led3\":\"0\",\"led4\":\"1\"}");
        Serial.println("Sending WebSocket Message");
    }

    digitalWrite(led, led_status);
}

void handleRoot(AsyncWebServerRequest* request)
{
    request->send(200, "text/html", index_html);
    Serial.println("Sending index.html");
}

void handleNotFound(AsyncWebServerRequest* request)
{
    String message = "File Not Found\n\n";
    message += "URI: ";
    message += request->url();
    message += "\nMethod: ";
    message += request->methodToString();
    message += "\nArguments: ";
    message += request->args();
    message += "\n";

    for (uint8_t i=0; i<request->args(); i++)
    {
        message += " " + request->argName(i) + ": " + request->arg(i) + "\n";
    }

    request->send(404, "text/plain", message);
}

void handleJson(AsyncWebServerRequest* request)
{
    String msg = "{";
    if (request->args())
    {
        int n = request->args();
        for (int i=0; i<n; i++)
        {
            msg += "\"" + request->argName(i) + "\":\"" + request->arg(i) + "\"";
            if (i != n-1)
            {
                msg += ",";
            }
        }
        msg += "}";
    }
    request->send(200, "application/json", msg);
    Serial.println("Sending json data");

    led_status = request->arg("led").compareTo("1")?1:0;
}

void handleJs(AsyncWebServerRequest* request)
{
    request->send(200, "application/javascript", index_js);
    Serial.println("Sending index.js");
}

void handleWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len)
{
    switch (type)
    {
        case WS_EVT_CONNECT:
            Serial.printf("WebSocket client #%u connected from %s\n", client->id(), client->remoteIP().toString().c_str());
            break;

        case WS_EVT_DISCONNECT:
            Serial.printf("WebSocket client #%u disconnected\n", client->id());
            break;
    }
}

void init_html(void)
{
    index_html += "<!DOCTYPE html>";
    index_html += "<html>";
    index_html += "<head>";
    index_html += "<title>Control Parqueo-matic Beta</title>";
    index_html += "<script src=\"index.js\"></script>";
    //index_html += "<link rel=\"stylesheet\" href=\"index.css\">";
    index_html += "</head>";
}

void init_js(void)
{
    index_js += "var gateway = `ws://${window.location.hostname}/ws`;\n";
    index_js += "var websocket;\n";
    index_js += "window.onload = function(){initWebSocket();};\n";
    index_js += "function initWebSocket(){\n";
    index_js += "websocket = new WebSocket(gateway);\n";
    index_js += "websocket.onmessage = onMessage;\n";
    index_js += "};\n";
    index_js += "function onMessage(event){\n";
    index_js += "let data = JSON.parse(event.data);";
    index_js += "console.log(`Received a notification from ${event.origin}`);\n";
    index_js += "console.log(\"LED1: \" + data.led1);\n";
    index_js += "console.log(\"LED2: \" + data.led2);\n";
    index_js += "console.log(\"LED3: \" + data.led3);\n";
    index_js += "console.log(\"LED4: \" + data.led4);\n";
    index_js += "};\n";
}
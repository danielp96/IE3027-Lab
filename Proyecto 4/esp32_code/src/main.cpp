
// call with web consoloe this, must be in / not in /json
// fetch("http://192.168.0.26/json?led1=true&led2=false&led3=false&led4=true", {method: "GET"}).then(res => res.json()).then(console.log)
//

// do not use delay, interferes with async
// use millis() instead

#include <Arduino.h>

#include <WiFiClient.h>
#include <WiFi.h>
#include <AsyncTCP.h>
#include "ESPAsyncWebServer.h"

const char* ssid = "";
const char* password = "";

AsyncWebServer server(80);
AsyncWebSocket ws("/ws");

const int led = 13;
int led_status = 1;

uint8_t data = 0;

String index_html;
String index_js  ;
String index_css ;

void handleRoot(AsyncWebServerRequest* request);
void handleNotFound(AsyncWebServerRequest* request);
void handleJson(AsyncWebServerRequest* request);
void handleJs(AsyncWebServerRequest* request);
void handleCss(AsyncWebServerRequest* request);
void init_html(void);
void init_js(void);
void init_css(void);
void handleWsEvent(AsyncWebSocket* server, AsyncWebSocketClient* client, AwsEventType type, void* arg, uint8_t* data, size_t len);

void setup(void)
{
    pinMode(led, OUTPUT);
    digitalWrite(led, 0);

    Serial.begin(115200);
    Serial2.begin(115200);

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
    init_css();

    ws.onEvent(handleWsEvent);
    server.addHandler(&ws);

    server.on("/", handleRoot);
    server.on("/index.js", handleJs);
    server.on("/index.css", handleCss);

    server.on("/json", handleJson);

    server.onNotFound(handleNotFound);

    server.begin();
    Serial.println("HTTP server started");
}

void loop(void)
{
    if (Serial2.available())
    {
        data = Serial2.read();

        String msg = "";

        msg += "{\"led1\":";
        msg += (data & 0x01)? "true" : "false";
        msg += ",";
        msg += "\"led2\":";
        msg += (data & 0x02)? "true" : "false";
        msg += ",";
        msg += "\"led3\":";
        msg += (data & 0x04)? "true" : "false";
        msg += ",";
        msg += "\"led4\":";
        msg += (data & 0x08)? "true" : "false";
        msg += "}";

        ws.textAll(msg);
        Serial.println("Sending WebSocket Message");

    }
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
    Serial.println("Not Found" + request->url());
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

void handleCss(AsyncWebServerRequest* request)
{
    request->send(200, "text/css", index_css);
    Serial.println("Sending index.css");
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
    index_html += "<meta charset=\"utf-8\">";
    index_html += "<script src=\"index.js\"></script>";
    index_html += "<link rel=\"stylesheet\" href=\"index.css\">";
    index_html += "</head>";
    index_html += "<body>";
    index_html += "<div id=\"panel\"><table>";
    index_html += "<tr><td id=\"car1\" class=\"display\">Parqueo 1</td></tr>";
    index_html += "<tr><td id=\"car2\" class=\"display\">Parqueo 2</td></tr>";
    index_html += "<tr><td id=\"car3\" class=\"display\">Parqueo 3</td></tr>";
    index_html += "<tr><td id=\"car4\" class=\"display\">Parqueo 4</td></tr>";
    index_html += "</table></div>";
    index_html += "</body>";
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
    index_js += "console.log(event.data);\n";
    index_js += "let data = JSON.parse(event.data);";
    index_js += "console.log(`Received a notification from ${event.origin}`);\n";
    index_js += "displayColor(\"car1\", data.led1);";
    index_js += "displayColor(\"car2\", data.led2);";
    index_js += "displayColor(\"car3\", data.led3);";
    index_js += "displayColor(\"car4\", data.led4);";
    index_js += "};\n";
    index_js += "function displayColor(id, value){\n";
    index_js += "var element = document.getElementById(id);\n";
    index_js += "if (value){element.style.backgroundColor = \"#ff6666\";\n}";
    index_js += "else{element.style.backgroundColor = \"#66ff66\";}\n";
    index_js += "}\n";
}

void init_css(void)
{
    index_css += "body{background-color: #e6e6e6;}";
    index_css += "#panel{";
    index_css += "position: absolute;";
    index_css += "width: 350px;";
    index_css += "background-color: white;";
    index_css += "border-radius: 4px;";
    index_css += "border: 1px solid #bfbfbf;";
    index_css += "padding: 2px;";
    index_css += "margin-left: calc(50% - 175px);";
    index_css += "}";
    index_css += ".display{";
    index_css += "border-radius: 3px;";
    index_css += "border: 1px solid #bfbfbf;";
    index_css += "padding: 3px;";
    index_css += "text-align: center;";
    index_css += "height: calc(80vh/4);";
    index_css += "}";
    index_css += "table{width: 100%;}";
    index_css += "/* mobile */";
    index_css += "@media only screen and (max-width: 1000px){";
    index_css += "#panel{";
    index_css += "width: 98%;";
    index_css += "margin-left: 0;";
    index_css += "}";
    index_css += ".display{";
    index_css += "height: calc(97vh/4);";
    index_css += "font-size: 45px;";
    index_css += "}";
    index_css += "}";
}
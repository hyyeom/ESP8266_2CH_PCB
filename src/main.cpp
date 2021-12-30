#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <Arduino_JSON.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_AM2320.h>
#include <SPI.h>
#include <LittleFS.h>

//const char *ssid = "EJD";
//const char *password = "5165135199ejd";
//const char *ssid = "N704";
//const char *password = "0123456789";

const char *ssid = "thyeom";
const char *password = "0432298555";

IPAddress ip(192, 168, 0, 101);
IPAddress gateway(192, 168, 0, 1);
IPAddress subnet(255, 255, 255, 0);
IPAddress dns(192, 168, 0, 1);

String myKey = "0";
const int SW1 = 15;   //switch  15
const int SW2 = 13;   //switch  13
const int ledPin = 2; //On board LED HIGH->OFF LOW->ON
String ledState;
AsyncWebServer server(80);
AsyncEventSource events("/events");
JSONVar readings;
// Timer variables
unsigned long lastTime = 0;
unsigned long timerDelay = 30000;

Adafruit_AM2320 AM2320 = Adafruit_AM2320();
void initAM2320()
{
  AM2320.begin();
}
void initLittleFS()
{
  if (!LittleFS.begin())
  {
    Serial.println("An error has occurred while mounting LittleFS");
  }
  Serial.println("LittleFS mounted successfully");
}
// Initialize WiFi
void initWiFi()
{
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  if (!WiFi.config(ip, gateway, subnet, dns))
  {
    Serial.println("Static IP Configuration Failed");
  }
  Serial.print("Connecting to WiFi ..");
  while (WiFi.status() != WL_CONNECTED)
  {
    Serial.print('.');
    delay(1000);
  }
  Serial.println(WiFi.localIP());
}
String SwState(int PinNum)
{
  if (digitalRead(PinNum))
  {
    ledState = "OFF";
  }
  else
  {
    ledState = "ON";
  }
  return ledState;
}
String processor(const String &var, int PinNum)
{
  if (var == "STATE")
  {
    if (digitalRead(PinNum))
    {
      ledState = "OFF";
    }
    else
    {
      ledState = "ON";
    }
    return ledState;
  }
  return String();
}
String getSensorReadings()
{
  readings["temperature"] = String(AM2320.readTemperature());
  readings["humidity"] = String(AM2320.readHumidity());
  readings["sw1state"] = String(SwState(SW1));
  readings["sw2state"] = String(SwState(SW2));
  readings["key"] = String(myKey);
  String jsonString = JSON.stringify(readings);
  return jsonString;
}
void winkLED(int n)
{
  for (int i = 0; i < n; i++)
  {
    digitalWrite(ledPin, LOW);
    delay(200);
    digitalWrite(ledPin, HIGH);
    delay(200);
  }
}
void setup()
{
  Serial.begin(9600);
  pinMode(SW1, OUTPUT);
  pinMode(SW2, OUTPUT);
  pinMode(ledPin, OUTPUT);
  digitalWrite(SW1, LOW);
  digitalWrite(SW2, LOW);
  digitalWrite(ledPin, HIGH);
  Serial.println("Start!!");
  initAM2320();
  Serial.println("AM2320");
  initWiFi();
  initLittleFS();

  // Web Server Root URL
  server.on("/", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              request->send(LittleFS, "/index.html", "text/html");
              Serial.println("send index.html");
            });

  server.serveStatic("/", LittleFS, "/");

  server.on("/control", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              request->send(LittleFS, "/control.html", "text/html");
              Serial.println("send control.html");
            });
  // Request for the latest sensor readings
  server.on("/readings", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              String json = getSensorReadings();
              request->send(200, "application/json", json);
              Serial.println(json);
              json = String();
              myKey = "0";
            });
  server.on("/readData", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              digitalWrite(ledPin, LOW);
              String json = getSensorReadings();
              request->send(200, "application/json", json);
              Serial.println(json);
              json = String();
              myKey = "0";
              delay(20);
              digitalWrite(ledPin, HIGH);
            });
  server.on("/login", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              //int pCnt = request->params();
              AsyncWebParameter *p = request->getParam(0);
              String pname = p->name();
              String rpass = p->value();
              if (rpass == "6227")
              {
                myKey = "6227";
              }
              else
              {
                myKey = "0";
              }
            });

  server.on("/onoffSw1", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              if (digitalRead(SW1))
              {
                digitalWrite(SW1, LOW);
              }
              else
              {
                digitalWrite(SW1, HIGH);
              }
              String json = getSensorReadings();
              request->send(200, "application/json", json);
              json = String();
            });

  server.on("/onoffSw2", HTTP_GET, [](AsyncWebServerRequest *request)
            {
              if (digitalRead(SW2))
              {
                digitalWrite(SW2, LOW);
              }
              else
              {
                digitalWrite(SW2, HIGH);
              }
              String json = getSensorReadings();
              request->send(200, "application/json", json);
              json = String();
            });

  server.onNotFound([](AsyncWebServerRequest *request)
                    { request->send(404, "text/plain", "The content you are looking for was not found."); });
  server.begin();

  winkLED(3);
}

void loop()
{
  if ((millis() - lastTime) > timerDelay)
  {
    // Send Events to the client with the Sensor Readings Every 10 seconds
    events.send("ping", NULL, millis());
    events.send(getSensorReadings().c_str(), "new_readings", millis());
    lastTime = millis();
  }
}
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#define BAUD_RATE 115200
#define SSID "POCOPHONE F1"
#define PASS "ggez1234"

#define RED_DELAY 30000
#define ORANGE_DELAY 20000
#define GREEN_DELAY 30000

String api_mine = "ZMD45I01TSDZ8B11";
String api_remote = "ZMD45I01TSDZ8B11";
String channel_mine = "1923880";
String serverNameWrite = "http://api.thingspeak.com/update?api_key=";
String serverNameRead = "http://api.thingspeak.com/channels/";

unsigned long timer;
bool state = false;

void setup() {
  Serial.begin(BAUD_RATE);

  WiFi.begin(SSID, PASS);
  Serial.println("Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.print("Connected to WiFi network with IP Address: ");
  Serial.println(WiFi.localIP());

  timer = millis();
}

void loop() {
  sendInt(0, RED_DELAY, api_mine, 1);
  sendInt(2, GREEN_DELAY, api_mine, 1);
  sendInt(1, ORANGE_DELAY, api_mine, 1);

  if (state == false && millis() - timer >= 600000) {
    state = true;
    sendInt(1, 0, api_mine, 8);
  }
  if (state && millis() - timer >= 60000) {
    state = false;
    sendInt(0, 0, api_mine, 8);
  }
}

void sendInt(int a, long d, String api_code, int field) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    Serial.print("Sending:");
    Serial.println(a);

    String serverPath = serverNameWrite + api_code + "&field" + field + "=" + a;

    Serial.println(serverPath);

    // Your Domain name with URL path or IP address with path
    http.begin(client, serverPath.c_str());

    // Send HTTP GET request
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
    delay(d);
  } else {
    Serial.println("WiFi Disconnected");
  }
}

int getInt(String channel_code, int field) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    String serverPath = serverNameRead + channel_code + "/fields/" + field + ".json?results=1";

    Serial.println(serverPath);

    // Your Domain name with URL path or IP address with path
    http.begin(client, serverPath.c_str());

    // Send HTTP GET request
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
      int index = payload.lastIndexOf("field" + String(field)) + 9;
      int number = payload.substring(index).toInt();
      return number;
    } else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
  } else {
    Serial.println("WiFi Disconnected");
  }

  return 0;
}

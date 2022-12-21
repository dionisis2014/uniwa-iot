#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>

#define BAUD_RATE 115200
#define SSID "IOT"
#define PASS "REDACTED"

#define RED_DELAY 30000
#define ORANGE_DELAY 20000
#define GREEN_DELAY 30000

//Your Domain name with URL path or IP address with path
String serverName = "http://api.thingspeak.com/update?api_key=ZMD45I01TSDZ8B11";

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
}

void loop() {
  sendInt(0,RED_DELAY);
  sendInt(2,GREEN_DELAY); 
  sendInt(1,ORANGE_DELAY);
}

void sendInt(int a, long d) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client;
    HTTPClient http;

    Serial.print("Sending:");
    Serial.println(a);

    String serverPath = serverName + "&field1=" + a;

    // Your Domain name with URL path or IP address with path
    http.begin(client, serverPath.c_str());

    // Send HTTP GET request
    int httpResponseCode = http.GET();

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
      String payload = http.getString();
      Serial.println(payload);
    }
    else {
      Serial.print("Error code: ");
      Serial.println(httpResponseCode);
    }
    // Free resources
    http.end();
    delay(d);
  }
  else {
    Serial.println("WiFi Disconnected");
  }
}

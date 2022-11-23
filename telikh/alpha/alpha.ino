#include <SoftwareSerial.h>
#include <avr/pgmspace.h>

#define HOST_SERIAL_BAUDRATE 115200

#define ESP_SERIAL_BAUDRATE 9600
#define ESP_SERIAL_PIN_RX 6
#define ESP_SERIAL_PIN_TX 7
#define ESP_SERIAL_BUFFER_SIZE 1024

#define WIFI_SSID "DION WiFi"
#define WIFI_PASSWORD "DION2000"

#define API_HOST "api.thingspeak.com"
#define API_PORT "80"
#define API_KEY_WRITE "ZMD45I01TSDZ8B11"
#define API_KEY_READ "OAEJ9C8HZ1GKIU8L"
#define API_FIELD "field1"

SoftwareSerial espSerial(ESP_SERIAL_PIN_RX, ESP_SERIAL_PIN_TX);
String rxBuffer;

void esp_cmd(const String& cmd, const uint16_t timeout) {
  uint32_t timer;

  Serial.print(F(">>> "));
  Serial.println(cmd);

  espSerial.println(cmd);

  rxBuffer = "";
  timer = millis();
  while (timer + timeout > millis()) {
    while (espSerial.available()) {
      rxBuffer += (char) espSerial.read();
    }
  }

  Serial.print("<<< ");
  Serial.println(rxBuffer);
}

void esp_send(int value) {
  String message = "GET /update?api_key=" API_KEY_WRITE "&" API_FIELD "=" + String(value) + "\r\n\r\n";

  esp_cmd("AT+CIPMUX=1", 1000);
  esp_cmd("AT+CIPSTART=0,\"TCP\",\"" API_HOST "\"," API_PORT, 1000);
  esp_cmd("AT+CIPSEND=0," + String(message.length()), 1000);

  espSerial.find(">");
  espSerial.println(message);

  esp_cmd("AT+CIPCLOSE=0", 1000);
}

void setup() {
  Serial.begin(HOST_SERIAL_BAUDRATE);
  espSerial.begin(ESP_SERIAL_BAUDRATE);

  esp_cmd("AT+RST", 1000);
  esp_cmd("AT+CWMODE=1", 1000);
#ifdef WIFI_PASSWORD
  esp_cmd("AT+CWJAP=\"" WIFI_SSID "\",\"" WIFI_PASSWORD "\"", 1000);
#else
  esp_cmd("AT+CWJAP=\"" WIFI_SSID "\",", 1000);
#endif
  while (!espSerial.find("WIFI GOT IP")) {
    Serial.print(".");
    delay(1000);
  }
}

void loop() {
  esp_send(random(1000));
  delay(10000);
}
#include <SoftwareSerial.h>      // Βιβλιοθήκη Software Serial
SoftwareSerial espSerial(6, 7);  // Pin 6: Συνδέεται με TX του ESP-01 (RX του Arduino)
                                 // Pin 7: Συνδέεται με RX του ESP-01 (TX του Arduino)
#define DEBUG true
String mySSID = "POCOPHONE F1";   // XXXΧΧX: To SSID του κοντινού WiFi
String myPWD = "ggez1234";  // pppppppp: Το password για τη σύνδεση στο WiFi
String myAPI = "ZMD45I01TSDZ8B11";      // QQQQ: To Write API Key από το κανάλι στο ThingSpeak
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELD = "field1";  // H μεταβλητή field1 στο κανάλι του ThingSpeak
int sendVal;

void setup() {
  Serial.begin(115200);
  espSerial.begin(9600);

  espData("AT+RST", 1000, DEBUG);                                         // ESP-01 reset
  espData("AT+CWMODE=1", 1000, DEBUG);                                    // Mode=1 => client
  espData("AT+CWJAP=\"" + mySSID + "\",\"" + myPWD + "\"", 1000, DEBUG);  // Σύνδεση στο WiFi
}

void loop() {
  sendVal = random(1000);  // Αποστολή τυχαίου αριθμού μεταξύ 1 και 1000
  String sendData = "GET /update?api_key=" + myAPI + "&" + myFIELD + "=" + String(sendVal);
  espData("AT+CIPMUX=1", 1000, DEBUG);
  espData("AT+CIPSTART=0,\"TCP\",\"" + myHOST + "\"," + myPORT, 1000, DEBUG);
  espData("AT+CIPSEND=0," + String(sendData.length() + 4), 1000, DEBUG);
  espSerial.find(">");
  espSerial.println(sendData);
  Serial.print("Value to be sent: ");
  Serial.println(sendVal);

  espData("AT+CIPCLOSE=0", 1000, DEBUG);
  delay(10000);
}

String espData(String command, const int timeout, boolean debug) {
  Serial.print("AT Command ==> ");
  Serial.print(command);
  Serial.println(" ");

  String response = "";
  espSerial.println(command);
  long int time = millis();
  while ((time + timeout) > millis()) {
    while (espSerial.available()) {
      char c = espSerial.read();
      response += c;
    }
  }
  if (debug) {
    //Serial.print(response);
  }
  return response;
}

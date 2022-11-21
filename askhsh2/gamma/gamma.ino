#include <SoftwareSerial.h>      // Βιβλιοθήκη Software Serial
SoftwareSerial espSerial(6, 7);  // Pin 6: Συνδέεται με TX του ESP-01 (RX του Arduino)
                                 // Pin 7: Συνδέεται με RX του ESP-01 (TX του Arduino)
#define DEBUG true
String mySSID = "POCOPHONE F1";  // XXXΧΧX: To SSID του κοντινού WiFi
String myPWD = "ggez1234";  // pppppppp: Το password για τη σύνδεση στο WiFi
String myAPI = "ZMD45I01TSDZ8B11";      // QQQQ: To Write API Key από το κανάλι στο ThingSpeak
String myReadAPI = "OAEJ9C8HZ1GKIU8L";  // WWWW: To Write API Key από το κανάλι στο ThingSpeak
String myHOST = "api.thingspeak.com";
String myPORT = "80";
String myFIELDa = "second";    // ALERT field
String myCHANNEL = "1923880";  // ID καναλιού στο ThingSpeak
unsigned int delayVal = 15000;
String response = "";
String res_feeds = "";
String messager;
String x01;

void setup() {
  Serial.begin(115200);
  espSerial.begin(9600);

  espData("AT+RST", 1000, DEBUG);                                         // ESP-01 Reset
  espData("AT+CWMODE=1", 1000, DEBUG);                                    // Mode=1 => client
  espData("AT+CWJAP=\"" + mySSID + "\",\"" + myPWD + "\"", 1000, DEBUG);  // Σύνδεση στο WiFi
  while (!espSerial.find("WIFI GOT IP"))                                  // Αναμονή σύνδεσης WiFi
  {
    Serial.print(".");
    delay(1000);
    Serial.print(".");
    delay(1000);
    Serial.print(".");
    delay(1000);
  }
  Serial.println("Connected!");  // Επιβεβαίωση σύνδεσης WiFi
  delay(1000);
}

void loop() {
  messager = "GET /channels/" + myCHANNEL + "/feeds.json?api_key=" + myReadAPI + "&results=1";
  espData("AT+CIPMUX=1", 1000, DEBUG);
  espData("AT+CIPSTART=0,\"TCP\",\"" + myHOST + "\"," + myPORT, 1000, DEBUG);
  espData("AT+CIPSTO=10", 1000, DEBUG);
  espData("AT+CIPSEND=0," + String(messager.length() + 4), 1000, DEBUG);
  espSerial.find(">");
  espSerial.println(messager);
  Serial.println("Starting to read...");
  espData("AT+CIPCLOSE=0", 1000, DEBUG);
  delay(1000);
  Serial.println(response);
  int ret_Len = response.length();

  int pos = response.indexOf("[");
  res_feeds = response.substring(pos, ret_Len);
  Serial.print(res_feeds);
  pos = res_feeds.indexOf(myFIELDa);
  x01 = res_feeds.substring(pos + 9, pos + 10);
  Serial.println("ALERT field value:" + x01);
  ret_Len = 0;
  x01 = "";
  pos = 0;
  res_feeds = "";
  delay(delayVal);
}

String espData(String command, const int timeout, boolean debug) {
  Serial.print("AT Command ==> ");
  Serial.print(command);
  Serial.println(" ");

  response = "";
  espSerial.println(command);
  long int time = millis();

  while ((time + timeout) > millis()) {
    while (espSerial.available()) {
      char c = espSerial.read();
      response += c;
    }
  }
  if (debug) {
    // Serial.print(response);
  }
  return response;
}
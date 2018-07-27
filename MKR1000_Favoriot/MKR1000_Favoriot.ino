#define _DISABLE_TLS_
#include <SPI.h>
#include <WiFi101.h>
#include "DHT.h" 

#define DHTPIN 6
#define DHTTYPE DHT11
DHT dht(DHTPIN,DHTTYPE);

char ssid[] = "your_network_SSID";      // change it!
char pass[] = "your_network_password";  // change it!
const String yourDevice = "deviceDefault@your_Username"; // change it!
int status = WL_IDLE_STATUS;
char server[] = "api.favoriot.com";
WiFiClient client;

void setup() {
  Serial.begin(115200);
  WiFi.disconnect();
  Serial.println("Mulai menghubungkan");
  WiFi.begin(ssid,pass);
  while((!(WiFi.status() == WL_CONNECTED))){
    delay(300);
    Serial.print("...");
  }
  Serial.println(WiFi.status());
  Serial.println("Terhubung!");
  Serial.println("");
}

void loop() {
  String pot = String(map(analogRead(A0), 0, 1023, 0, 100));
  String temp = String(dht.readTemperature());
  String humid = String(dht.readHumidity());
  String json = "{\"device_developer_id\":\""+yourDevice+"\",\"data\":{\"Potentio\":\""+pot+"\",\"Temperature\":\""+temp+"\",\"Humidity\":\""+humid+"\"}}";
  Serial.println(json);
  if (client.connect(server, 80)) {
    
    client.println("POST /v1/streams HTTP/1.1");
    client.println("Host: api.favoriot.com");
    client.println(F("apikey: your_API_key"));  // change it!
    client.println("Content-Type: application/json");
    client.println("cache-control: no-cache");
    client.print("Content-Length: ");
    int thisLength = json.length();
    client.println(thisLength);
    client.println("Connection: close");

    client.println();
    client.println(json);
  }
  delay(5000);
}
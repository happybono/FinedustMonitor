//
//    FILE: server.ino
//  AUTHOR: Jaewoong Mun (happybono@outlook.com)
// CREATED: November 19, 2019
//
// Released to the public domain
//

const char* host = "data.plaive.10make.com";
String url = "/new_chart_line?api_key=";  
const int httpPort = 80;

#include <ESP8266WiFi.h>
#include <WiFiClient.h>

void do_server(String APIKEY, int pm25, int pm10, float temperature) { 
  WiFiClient client;

  if (!client.connect(host, httpPort)) {
    Serial.print("connection failed: ");
    return;
  }

  String payload = "pm25="+ String(pm25) +"&pm10="+ String(pm10)+"&temperature="+ String(temperature);
  String getheader = "GET "+ String(url) + String(APIKEY)+ "&"+ payload +" HTTP/1.1";
  client.println(getheader);
  client.println("User-Agent: ESP8266 HAPPYBONO-DMK");  
  client.println("Host: " + String(host));  
  client.println("Connection: close");  
  client.println();

  Serial.println(getheader);
  while (client.connected()) {
    String line = client.readStringUntil('\n');
    Serial.println(line);
  }
  Serial.println("Send to Server.");
}

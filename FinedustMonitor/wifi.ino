//
//    FILE: wifi.ino
//  AUTHOR: Jaewoong Mun (happybono@outlook.com)
// CREATED: November 19, 2019
//
// Released to the public domain
//

#include <ESP8266WiFi.h>

boolean connect_ap(char* ssid, char* password) {
  int count = 60;
  Serial.println();
  Serial.print("connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
    wifi_oled(count);
    if (!count--) {
      Serial.print("\nNO WIFI");
      return(false);
    }
  }
  Serial.print("\n Got WiFi, IP address: ");
  Serial.println(WiFi.localIP()); 
  return(true);
}

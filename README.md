# FinedustMonitor
Fine dust (air pollution) monitoring station. Air pollution monitoring channel updates once every minute with PM10.0 and PM2.5 readings from the Nova PM (SDS011) Sensor.  

> **[Please note]** </br>
This project doesn't support GPS and realtime location tracking capability. For a project that supports GPS and realtime location tracking, please visit the following link : https://github.com/happybono/FinedustMonitorWithGPS

![Final Product](20191212_085317.jpg)

## What's New
### November 24, 2019
Initial Release.

### December 27, 2019
[Now supports [Channel Status Updates] in ThingSpeak](https://thingspeak.com/channels/920137) : Updated [FinedustMonitorWithGPS.ino](https://github.com/happybono/FinedustMonitor/commit/2c67b2f0da2b040965968acb8d0f28a61f4c0c7f "/FinedustMonitor/FinedustMonitor.ino"), [server.ino](https://github.com/happybono/FinedustMonitor/commit/0f668a0b3ce72a1ad0b89f1f80e4df6fb47ed87a "/FinedustMonitor/server.ino")

### January 25, 2020
["Latest PM sensor readings" plugin added in ThingSpeak.](https://github.com/happybono/FinedustMonitor/tree/master/Plugins/StatusUpdates)

### January 26, 2020
Various bugs fixed.

### February 20, 2020
["Email Alert" function added as a ThingSpeak Analysis plug-in.](https://github.com/happybono/FinedustMonitor/blob/master/Plugins/Email/EmailNotifier.m) 

## Specifications
### Scenarios
* Measuring the fine dust contamination level in the air in real time. The value is displayed on the OLED Screen, and updated every second.
* Measuring the current temperature.
* Refining data and synchronize with the web-based cloud service. (e.g. [ThingSpeak](https://www.thingspeak.com/), [Plaive](https://plaive.10make.com/))

### Connections
* 1 x Micro-USB

### Wireless
* IEEE 802.11 b/g/n Wi-Fi technology.

### Battery life 
* Up to 24 hours of typical device usage.

## Apparatus (Equipment)
### Platform
* ESP8266 NodeMCU

### SDS011 Dust Sensor
* Soldering required.
* D1 : TX of SDS011

### 0.96" I2C OLED Display 
* Soldering required.
* D3 : Data, D2 : Clock

### DS18820 Temperature Sensor (Thermometer)
* Soldering required.
* D4 : Data

### Blank EUB-2300C Portable Battery
* Micro-USB : Power 
* Dimension : W (66.4 mm) × D (95.0 mm) × H (6.5 mm) 
* Input : DC-5V / 0.6A
* Output : DC-5V / 0.6A
* Capacity : 2,300 mAh

## Data Refinement / Synchronization
* ThingSpeak (https://thingspeak.com/)
* Plaive (https://plaive.10make.com/)

## Drivers
Please install https://github.com/squix78/esp8266-oled-ssd1306

## References
Not applicable.

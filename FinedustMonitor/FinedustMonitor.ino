//
//    FILE: FinedustMonitor.ino
//  AUTHOR: Jaewoong Mun (happybono@outlook.com)
// CREATED: November 19, 2019
//
// Released to the public domain
//

#include <SoftwareSerial.h>
SoftwareSerial dust(D1, D0, false, 256);

#include "RunningMedian.h"
RunningMedian pm25s = RunningMedian(19);
RunningMedian pm10s = RunningMedian(19);

const int INTERVAL = 60000;

String api_key = ""; // API KEY
char* ssid = "";
char* password = "";
const int RATIO = 10;

//#define PLAIVE_SERVER_ENABLE
#define THINGSPEAK_SERVER_ENABLE

boolean wifi_ready;
String status;

void setup() {
  Serial.begin(9600);
  dust.begin(9600);
  setup_oled();
  wifi_ready = connect_ap(ssid, password);
  if (!wifi_ready) nowifi_oled();

  Serial.println("\nDust Sensor Box V1.2, 2019/12/25 HAPPYBONO");
}

void got_dust(int pm25, int pm10) {
   pm25 /= RATIO;
   pm10 /= RATIO;
   //Serial.println(String(pm25) +","+ String(pm10)+ "  ");
   pm25s.add(pm25);
   pm10s.add(pm10);
   //Serial.println("pm25 size="+ String(pm25s.getSize()) +", count="+ String(pm25s.getCount()) +", median="+ String(pm25s.getMedian()));
   //Serial.println("pm10 size="+ String(pm10s.getSize()) +", count="+ String(pm10s.getCount()) +", median="+ String(pm10s.getMedian()));
   do_oled(pm25, pm10);
}

void do_interval() {
  if (wifi_ready) {
#ifdef PLAIVE_SERVER_ENABLE
    do_server_plaive(api_key, int(pm25s.getMedian()), int(pm10s.getMedian()), get_temperature());
#else
#ifdef THINGSPEAK_SERVER_ENABLE
    Serial.println("dst: pm25=" + String(int(pm25s.getMedian())) + " / pm10=" + String(int(pm10s.getMedian())) + "/ s=" + String(status));
    do_server_thingspeak(api_key, int(pm25s.getMedian()), int(pm10s.getMedian()), get_temperature(), status);
#else
    do_server_default(api_key, int(pm25s.getMedian()), int(pm10s.getMedian()), get_temperature());
#endif
#endif
  }                                                    //wifi is ok
}

unsigned long mark = 0;
boolean got_interval = false;
  
int pm25i, pm10i;
//아두이노가 반복적으로 작동하는 부분 (Where Arduino works repeatedly.)
void loop() {
  if (millis() > mark) {
    mark = millis() + INTERVAL;
    got_interval = true;
  }
  while (dust.available() > 0) {
    do_dust(dust.read(), got_dust);
    yield();                                          //loop 에서 while 문을 사용하는 경우 yield 를 포함해주어야 합니다.

    //Serial.println(map_x);
    //Serial.print("pm 10 : ");
    //Serial.println(int(pm10s.getMedian()));

    /* AQI (실시간 대기질 지수) 등급 분류를 위한 코드입니다.
       실시간 대기질 기준 수치는 국제 표준인 WHO 대기질 수치 기준으로 분류합니다.

       http://www.euro.who.int/__data/assets/pdf_file/0005/78638/E90038.pdf
       https://airnow.gov/index.cfm?action=aqibasics.aqi */


    // 초미세먼지 AQI (실시간 대기질 지수) 등급을 분류합니다.
    //   0 이상   8 이하 : 1
    //   9 이상  16 이하 : 2
    //  17 이상  26 이하 : 3
    //  27 이상  34 이하 : 4
    //  35 이상  43 이하 : 5
    //  44 이상  51 이하 : 6
    //  52 이상  ∞  이하 : 7

    int pm25m = int(pm25s.getMedian());

    if (pm25m < 9) {
      pm25i = 1;
    } else if (pm25m < 17) {
      pm25i = 2;
    } else if (pm25m < 27) {
      pm25i = 3;
    } else if (pm25m < 35) {
      pm25i = 4;
    } else if (pm25m < 44) {
      pm25i = 5;
    } else if (pm25m < 52) {
      pm25i = 6;
    } else {
      pm25i = 7;
    }

    // 미세먼지 AQI (실시간 대기질 지수) 등급을 분류합니다.
    //   0 이상  16 이하 : 1
    //  16 이상  31 이하 : 2
    //  32 이상  51 이하 : 3
    //  52 이상  68 이하 : 4
    //  69 이상  84 이하 : 5
    //  85 이상 101 이하 : 6
    // 102 이상  ∞  이하 : 7

    int pm10m = int(pm10s.getMedian());

    if (pm10m < 16) {
      pm10i = 1;
    } else if (pm10m < 32) {
      pm10i = 2;
    } else if (pm10m < 52) {
      pm10i = 3;
    } else if (pm10m < 69) {
      pm10i = 4;
    } else if (pm10m < 85) {
      pm10i = 5;
    } else if (pm10m < 102) {
      pm10i = 6;
    } else {
      pm10i = 7;
    }

    /* ThingSpeak 채널 내 Status Update (상태 업데이트) 영역에 표시되는 문구이므로,
        종합적인 정보 표현을 위해 초미세먼지와 미세먼지 등급을 비교 한 후
        두 가지 중 높은 등급 기준으로 경고 혹은 권고메시지를 표시합니다. */

    // 분류된 초미세먼지 등급이 미세먼지 등급보다 같거나 높은 경우, 초미세먼지 등급을 기준으로 내용을 표시하기 위하여 아래의 문자열을 status 변수에 저장합니다.

    switch ((pm25i >= pm10i) ? pm25i : pm10i) {
      case 1:
        status = "Excellent%20(1)%20:%20The%20air%20quality%20is%20excellent.%20As%20air%20pollution%20poses%20no%20threat,%20conditions%20are%20ideal%20for%20outdoor%20activities.";
        break;

      case 2:
        status = "Very%20Good%20(2)%20:%20The%20air%20quality%20is%20very%20good.%20As%20air%20pollution%20poses%20little%20or%20no%20risk,%20conditions%20very%20good%20for%20outdoor%20activities.";
        break;

      case 3:
        status = "Moderate%20(3)%20:%20Air%20quality%20is%20acceptable.%20however,%20for%20some%20pollutants,%20there%20may%20be%20a%20moderate%20health%20concern%20for%20specific%20people%20who%20are%20unusually%20sensitive%20to%20air%20pollution.";
        break;

      case 4:
        status = "Satisfactory%20(4)%20:%20Members%20of%20sensitive%20groups%20may%20experience%20health%20effects,%20other%20people%20should%20limit%20spending%20time%20outdoors,%20especially%20when%20they%20experience%20symptoms%20such%20as%20cough%20or%20sore%20throat.";
        break;

      case 5:
        status = "Bad%20(5)%20:%20Everyone%20may%20begin%20to%20experience%20health%20effects,%20members%20of%20sensitive%20groups%20may%20experience%20more%20serious%20health%20effects.%20Not%20recommended%20for%20outdoor%20activities.";
        break;

      case 6:
        status = "Severe%20(6)%20:%20Everyone%20may%20experience%20more%20serious%20health%20effects.%20People%20at%20risk%20should%20be%20avoided%20to%20go%20outside%20and%20should%20limit%20outdoor%20activities%20to%20a%20minimum.%20Outdoor%20activities%20are%20discouraged.";
        break;

      case 7:
        status = "Hazardous%20(7)%20:%20People%20at%20risk%20should%20avoid%20going%20outside%20and%20should%20limit%20outdoor%20activities%20to%20a%20minimum.%20Outdoor%20activities%20are%20strongly%20discouraged.";
        break;
    }

    //Serial.println("PM2.5 = " + String(int(pm25s.getMedian())) + " / " + String(pm25i));
    //Serial.println("PM10.0  = " + String(int(pm10s.getMedian())) + " / " + String(pm10i));
  }

  if (millis() > mark) {                          //one minute (60000) interval
    mark = millis() + 60000;
    got_interval = true;
  }

  if (got_interval) {
    got_interval = false;
    do_interval();
  }
  yield();
}

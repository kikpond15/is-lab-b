#include <Arduino.h>
#include <M5Unified.h>
#include "HCSR04.h"
#include <SD.h>
File sdcard;
bool loggingState = false;
const int trig_pin = G25;
const int echo_pin = G35;
HCSR04 hcsr04;

void saveSD(int dist);

void setup() {
  m5::M5Unified::config_t cfg = M5.config();
  M5.begin(cfg);
  M5.Display.setTextSize(2);
  M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Display.clear();
  M5.Display.drawString("Logging Stop", 0, 30);

  hcsr04.begin(trig_pin, echo_pin);

  SD.begin(4, SPI, 40000000);  
  sdcard = SD.open("/m5.csv", FILE_WRITE);
  sdcard.println("time, diastance");
  sdcard.flush();
}

void loop() {
  M5.update();
  float dist = hcsr04.distance();
  M5.Display.setCursor(0, 0);
//  M5.Display.printf("distance: %6.2fcm", dist);
  M5.Display.drawString("distance: " + String(dist) + "cm" , 0, 50);

  if (M5.BtnA.wasPressed()){  // ボタンAが押されたらログの開始/停止を切り替え
    if(loggingState){
      loggingState = false;
      M5.Display.drawString("Logging Stop ", 0, 30);
    } else{
      loggingState = true;
      M5.Display.drawString("Logging Start", 0, 30);
    }
  }

  // ログの状態がONならタッチ位置をSDカードに保存
  if (loggingState == true) saveSD(dist);
  delay(10);
}

void saveSD(int dist) {
  String line = String(millis()) + "," + String(dist);
  if (sdcard) {
    sdcard.println(line);// 書き込み
    sdcard.flush();// SDカードに保存
  }
}
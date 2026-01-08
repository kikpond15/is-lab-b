/*
3ピンのDHT11センサを使用する。
Library : "DHT sensor library for ESPx"

下記の並びで配線
GPIO26, 3.3V, GND
*/

#include <M5Unified.h>
#include "DHTesp.h"

DHTesp dht;

void setup() {
  M5.begin();
  Serial.begin(9600);
  dht.setup(26, DHTesp::DHT11); // DHT22センサーをGPIO17に接続
}

void loop() {
  // 温度と湿度の測定
  delay(dht.getMinimumSamplingPeriod());

  float humidity = dht.getHumidity();
  float temperature = dht.getTemperature();

  // 測定結果を画面に表示
  
  M5.Lcd.setCursor(0, 60);
  M5.Lcd.setTextSize(3);
  M5.Lcd.printf("Temp: %.1f C", temperature);
  M5.Lcd.setCursor(0, 100);
  M5.Lcd.setTextSize(3);
  M5.Lcd.printf("Hum : %.1f %%", humidity);

  // 1分（60000ミリ秒）待機
  // delay(3000);
  M5.Lcd.clearDisplay();
}
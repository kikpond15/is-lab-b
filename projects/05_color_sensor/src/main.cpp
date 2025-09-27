// https://tamanegi-digick.com/it/tcs34725-2/
// https://note.com/kumin/n/n69bc23222c46

/* 接続例（M5StackではI2Cは内部配線済み）
   SCL → GPIO33
   SDA → GPIO32
   VDD → 3.3V
   GND → GND
*/

/* センサ初期化
   デフォルト値（積分時間2.4ms, gain=1x）なら
   Adafruit_TCS34725 tcs = Adafruit_TCS34725();
*/

#include <Wire.h>                     // I2C通信ライブラリ
#include <Adafruit_TCS34725.h>         // カラーセンサTCS34725用ライブラリ
#include <M5Unified.h>                 // M5Stack用統合ライブラリ

// 積分時間614ms・ゲイン1xで初期化（高精度測定）
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);


void setup(void) {
  Serial.begin(9600);
  M5.begin();
  M5.Lcd.setRotation(1);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(WHITE);
  // カラーセンサ初期化
  if (tcs.begin()) {               // センサが見つかった場合
    M5.Lcd.setCursor(10, 10);
    M5.Lcd.println("TCS34725 OK");
  } else {                         // センサが見つからなかった場合               
    M5.Lcd.setCursor(10, 10); 
    M5.Lcd.println("Sensor Error");
    while (1);
  }
}

void loop(void) {
  uint16_t r, g, b, c, colorTemp, lux;  // 生データ格納用変数
  tcs.getRawData(&r, &g, &b, &c); // 生データ取得（赤・緑・青・クリア）
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c); // 色温度・照度の計算（必要に応じて使える）
  lux = tcs.calculateLux(r, g, b);

  uint8_t R, G, B;  // 明るさ（クリア値c）で正規化して0-255のRGB値に変換
  if (c > 0) {
    R = (uint32_t)r * 255 / c; // 赤成分を正規化
    G = (uint32_t)g * 255 / c; // 緑成分を正規化
    B = (uint32_t)b * 255 / c; // 青成分を正規化
  } else {
    R = G = B = 0; // クリア値0のときは0
  }

  // 表示更新
  M5.Lcd.fillRect(0, 50, 320, 200, BLACK);
  M5.Lcd.setCursor(10, 60);
  M5.Lcd.printf("R: %d", R);
  M5.Lcd.setCursor(10, 100);
  M5.Lcd.printf("G: %d", G);
  M5.Lcd.setCursor(10, 140);
  M5.Lcd.printf("B: %d", B);

  //uint16_t COLOR= M5.Lcd.color565(R,G,B);
  //M5.Lcd.fillRect(0, 50, 320, 200, COLOR);
  delay(500);
}

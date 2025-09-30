#include <Wire.h>
#include <Adafruit_TCS34725.h>
#include <M5Unified.h>
#include <SD.h>
Adafruit_TCS34725 tcs = Adafruit_TCS34725(TCS34725_INTEGRATIONTIME_614MS, TCS34725_GAIN_1X);
File sdcard;

void saveColor(uint32_t r, uint32_t g, uint32_t b);

void setup(void)
{
  Serial.begin(9600);
  M5.begin();
  M5.Lcd.setRotation(1);
  M5.Lcd.setTextSize(2);
  M5.Lcd.fillScreen(WHITE);
  M5.Lcd.setTextColor(BLACK);

  tcs.begin();

  SD.begin(4, SPI, 40000000);                    // SDカード初期化(CSピン4, SPIクロック40MHz)
  sdcard = SD.open("/m5_color.csv", FILE_WRITE); // 追記モードでオープン
  sdcard.println("r,g,b");                       // ヘッダー行書き込み
  sdcard.flush();
}

void loop(void)
{
  M5.update();
  uint16_t r, g, b, c, colorTemp, lux;
  tcs.getRawData(&r, &g, &b, &c);
  colorTemp = tcs.calculateColorTemperature_dn40(r, g, b, c);
  lux = tcs.calculateLux(r, g, b);

  uint8_t R, G, B; // 明るさ（クリア値c）で正規化して0-255のRGB値に変換
  if (c > 0)
  {
    R = (uint32_t)r * 255 / c; // 赤成分を正規化
    G = (uint32_t)g * 255 / c; // 緑成分を正規化
    B = (uint32_t)b * 255 / c; // 青成分を正規化
  }
  else
  {
    R = G = B = 0;
  }

  uint16_t COLOR = M5.Lcd.color565(R, G, B);
  M5.Lcd.fillRect(0, 30, 320, 300, COLOR);

  M5.Display.drawString("R:" + String(R) + "  ", 10, 60);
  M5.Display.drawString("G:" + String(G) + "  ", 10, 90);
  M5.Display.drawString("B:" + String(B) + "  ", 10, 120);

  if (M5.BtnA.wasPressed())
  {
    saveColor(R, G, B); // ボタンAが押されたら保存
    M5.Display.drawString("Saved Color", 10, 10);
    delay(2000);
    M5.Lcd.fillRect(0, 0, 320, 30, WHITE);
  }
  //delay(50);
}

void saveColor(uint32_t r, uint32_t g, uint32_t b)
{
  // CSV形式で「時刻,ボタン」書き込み(時刻は起動からのミリ秒)
  String line = String(r) + "," + String(g) + "," + String(b);
  if (sdcard)
  {
    sdcard.println(line); // 書き込み
    sdcard.flush();       // SDカードに保存
  }
}
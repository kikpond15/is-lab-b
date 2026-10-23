// main.cpp (PlatformIO)
// CalibratedColorSensor クラスを使った M5Stack 用サンプル
//   BtnA … 白キャリブレーション（白い紙に当てて押す）
//   BtnB … 黒キャリブレーション（黒い物に当てて押す）
//   BtnC … キャリブレーションをリセット

#include <Arduino.h>
#include <M5Unified.h>
#include "CalibratedColorSensor.h"

CalibratedColorSensor sensor(TCS34725_INTEGRATIONTIME_154MS, TCS34725_GAIN_4X);

void showMessage(const char *msg, uint16_t color) {
  M5.Display.fillScreen(BLACK);
  M5.Display.setTextColor(color, BLACK);
  M5.Display.setCursor(10, 100);
  M5.Display.println(msg);
  delay(800);
}

void setup() {
  M5.begin();
  Serial.begin(115200);
  M5.Display.setRotation(1);
  M5.Display.setTextSize(2);

  Wire.begin(M5.Ex_I2C.getSDA(), M5.Ex_I2C.getSCL());   // PortA
  if (!sensor.begin(Wire)) {
    showMessage("Sensor Error", RED);
    while (1) delay(100);
  }
  sensor.setAverageCount(4);

  if (!sensor.isCalibrated()) showMessage("Press A on white", YELLOW);
}

void loop() {
  M5.update();

  if (M5.BtnA.wasPressed()) {
    bool ok = sensor.calibrateWhite();
    showMessage(ok ? "White calibrated" : "Saturated! lower gain", ok ? GREEN : RED);
  }
  if (M5.BtnB.wasPressed()) { sensor.calibrateBlack();   showMessage("Black calibrated", GREEN); }
  if (M5.BtnC.wasPressed()) { sensor.resetCalibration(); showMessage("Calibration reset", YELLOW); }

  auto c = sensor.read();

  int w = M5.Display.width(), h = M5.Display.height();
  M5.Display.fillRect(0, 0, w, h - 50, c.toRGB565());

  M5.Display.setTextColor(WHITE, BLACK);
  M5.Display.setCursor(5, h - 45);
  M5.Display.printf("R%3d G%3d B%3d  #%06lX ", c.r, c.g, c.b, (unsigned long)c.toHex());
  M5.Display.setCursor(5, h - 22);
  M5.Display.setTextColor(c.saturated ? RED : WHITE, BLACK);
  M5.Display.printf("C:%5d %s        ", (int)c.clear, c.saturated ? "SATURATED" : "");

  Serial.printf("raw %.0f,%.0f,%.0f C%.0f -> %d,%d,%d\n",
                c.rawR, c.rawG, c.rawB, c.clear, c.r, c.g, c.b);
}

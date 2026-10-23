// M5Stack Heart Rate Unit (MAX30100) サンプル
// - 心拍数 (BPM) と SpO2 を表示
// - 拍動のたびにハートマークが点滅
// - 心拍数の推移をグラフ表示
//
// 注意：
// - MAX30100 は FIFO が 16 サンプル（100Hz で約 160ms）しかないため、
//   pox.update() を頻繁に呼ぶ必要がある。loop() に delay() を入れないこと。
//   画面描画も間引いて短時間で終わらせている。
// - SpO2 は較正されていない目安値。医療用途には使えない。
// - 指は強く押し付けず、軽く乗せて動かさないのがコツ。

#include <Arduino.h>
#include <M5Unified.h>
#include <Wire.h>
#include "MAX30100_PulseOximeter.h"

PulseOximeter pox;
M5Canvas canvas(&M5.Display);

// ---- 設定 ----
const uint32_t DRAW_INTERVAL_MS  = 250;   // 画面更新の間隔
const uint32_t GRAPH_INTERVAL_MS = 1000;  // グラフに1点追加する間隔
const uint32_t BEAT_FLASH_MS     = 150;   // ハートを赤くする時間
const int      GRAPH_POINTS      = 60;    // グラフの点数（=60秒分）
const float    GRAPH_MIN_BPM     = 40;
const float    GRAPH_MAX_BPM     = 160;

// ---- 状態 ----
volatile uint32_t lastBeatMs = 0;
float bpmHistory[GRAPH_POINTS];
int   historyCount = 0;
uint32_t lastDrawMs = 0, lastGraphMs = 0;

// 拍動検出時に呼ばれる（短い処理だけにする）
void onBeatDetected() {
  lastBeatMs = millis();
}

void pushHistory(float bpm) {
  if (historyCount < GRAPH_POINTS) {
    bpmHistory[historyCount++] = bpm;
  } else {
    memmove(bpmHistory, bpmHistory + 1, sizeof(float) * (GRAPH_POINTS - 1));
    bpmHistory[GRAPH_POINTS - 1] = bpm;
  }
}

void drawHeart(int cx, int cy, int s, uint16_t color) {
  canvas.fillCircle(cx - s / 2, cy - s / 4, s / 2, color);
  canvas.fillCircle(cx + s / 2, cy - s / 4, s / 2, color);
  canvas.fillTriangle(cx - s, cy - s / 8, cx + s, cy - s / 8, cx, cy + s, color);
}

void drawScreen(float bpm, float spo2, bool fingerOn, bool beatNow) {
  const int W = canvas.width();
  const int H = canvas.height();
  canvas.fillScreen(TFT_BLACK);

  // ハート
  drawHeart(40, 45, 18, beatNow ? TFT_RED : 0x4000);

  if (!fingerOn) {
    canvas.setTextColor(TFT_YELLOW);
    canvas.setFont(&fonts::FreeSansBold12pt7b);
    canvas.drawString("Place your finger", 80, 35);
  } else {
    // BPM
    canvas.setTextColor(TFT_WHITE);
    canvas.setFont(&fonts::FreeSansBold24pt7b);
    canvas.drawString(String((int)(bpm + 0.5f)).c_str(), 80, 20);
    canvas.setFont(&fonts::FreeSans9pt7b);
    canvas.drawString("BPM", 160, 45);

    // SpO2（0 は未計算）
    canvas.setFont(&fonts::FreeSansBold12pt7b);
    canvas.setTextColor(TFT_CYAN);
    String s = (spo2 > 0) ? String((int)spo2) + "%" : String("--%");
    canvas.drawString(("SpO2 " + s).c_str(), 205, 25);
  }

  // グラフ
  const int gx = 10, gy = 90, gw = W - 20, gh = H - gy - 10;
  canvas.drawRect(gx, gy, gw, gh, TFT_DARKGREY);
  canvas.setFont(&fonts::Font0);
  canvas.setTextColor(TFT_DARKGREY);
  canvas.drawString(String((int)GRAPH_MAX_BPM).c_str(), gx + 2, gy + 2);
  canvas.drawString(String((int)GRAPH_MIN_BPM).c_str(), gx + 2, gy + gh - 10);

  auto toY = [&](float v) {
    v = constrain(v, GRAPH_MIN_BPM, GRAPH_MAX_BPM);
    return gy + gh - 1 - (int)((v - GRAPH_MIN_BPM) / (GRAPH_MAX_BPM - GRAPH_MIN_BPM) * (gh - 2));
  };
  for (int i = 1; i < historyCount; i++) {
    if (bpmHistory[i - 1] <= 0 || bpmHistory[i] <= 0) continue;  // 指なし区間は描かない
    int x0 = gx + (i - 1) * gw / GRAPH_POINTS;
    int x1 = gx + i * gw / GRAPH_POINTS;
    canvas.drawLine(x0, toY(bpmHistory[i - 1]), x1, toY(bpmHistory[i]), TFT_RED);
  }

  canvas.pushSprite(0, 0);
}

void setup() {
  M5.begin();
  Serial.begin(115200);
  M5.Display.setRotation(1);

  canvas.setColorDepth(8);  // メモリ節約（PSRAM 無しの機種でも動くように）
  canvas.createSprite(M5.Display.width(), M5.Display.height());

  // PortA の I2C を先に初期化（ライブラリ内の Wire.begin() は既に初期化済みなら無視される）
  Wire.begin(M5.Ex_I2C.getSDA(), M5.Ex_I2C.getSCL());

  if (!pox.begin()) {
    M5.Display.setTextColor(TFT_RED);
    M5.Display.setTextSize(2);
    M5.Display.drawString("MAX30100 not found", 10, 10);
    while (1) delay(100);
  }

  // IR LED の電流。値が飽和する／弱すぎるときは調整（7.6mA 前後が目安）
  pox.setIRLedCurrent(MAX30100_LED_CURR_7_6MA);
  pox.setOnBeatDetectedCallback(onBeatDetected);
}

void loop() {
  pox.update();   // できるだけ頻繁に呼ぶ（delay 禁止）
  M5.update();

  uint32_t now = millis();
  float bpm  = pox.getHeartRate();   // 指が無い／検出前は 0
  float spo2 = pox.getSpO2();        // 未計算時は 0
  bool fingerOn = bpm > 0;

  if (now - lastGraphMs >= GRAPH_INTERVAL_MS) {
    lastGraphMs = now;
    pushHistory(fingerOn ? bpm : 0);
    Serial.printf("BPM: %.1f  SpO2: %.0f%%\n", bpm, spo2);
  }

  if (now - lastDrawMs >= DRAW_INTERVAL_MS) {
    lastDrawMs = now;
    bool beatNow = (now - lastBeatMs) < BEAT_FLASH_MS + DRAW_INTERVAL_MS;
    drawScreen(bpm, spo2, fingerOn, beatNow);
  }

  // BtnA でグラフをクリア
  if (M5.BtnA.wasPressed()) historyCount = 0;
}

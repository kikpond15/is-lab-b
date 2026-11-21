// 心拍センサ（アナログ出力）から心拍数 (BPM) を計算して表示する
// https://www.switch-science.com/products/1135
//
// 仕組み：
//  1. 2msごと(500Hz)にアナログ値を読む
//  2. 波形の最大/最小から「しきい値」を自動調整する
//  3. 波形がしきい値を下から上に横切った瞬間を「拍動」とみなす
//  4. 拍動の間隔(IBI: ms)の平均から BPM = 60000 / IBI を求める
//
// 接続：センサの信号線 → GPIO36 (ADC1_CH0)
// 指は強く押し付けず、軽く乗せて動かさないのがコツ。

#include <M5Unified.h>

// ---- 設定 ----
const int      SENSOR_PIN       = 36;     // GPIO36(ADC1_CH0)
const uint32_t SAMPLE_US        = 2000;   // サンプリング間隔 2ms
const uint32_t DRAW_INTERVAL_MS = 500;    // 画面更新の間隔
const uint32_t PLOT_INTERVAL_MS = 10;     // Teleplotへの出力間隔
const uint32_t NO_BEAT_RESET_MS = 2500;   // この時間拍動が無ければリセット
const int      IBI_COUNT        = 8;      // 平均に使う拍動間隔の数
const int      MIN_IBI_MS       = 300;    // 200BPM 相当（これより短い間隔は無視）
const int      MAX_IBI_MS       = 2000;   // 30BPM 相当（これより長い間隔は無視）

// ---- 状態 ----
int      thresh   = 2048;   // しきい値
int      peak     = 2048;   // 直近の山
int      trough   = 2048;   // 直近の谷
bool     pulse    = false;  // しきい値より上にいる間 true
uint32_t lastBeatMs = 0;    // 前回の拍動時刻
bool     hasLastBeat = false;

int      ibiBuf[IBI_COUNT];
int      ibiNum = 0;        // バッファに入っている数
int      ibiPos = 0;
int      bpm    = 0;

uint32_t lastSampleUs = 0;
uint32_t lastDrawMs   = 0;
uint32_t lastPlotMs   = 0;
int      lastSignal   = 0;

void resetDetector(uint32_t now) {
  thresh = peak = trough = 2048;
  pulse = false;
  hasLastBeat = false;
  lastBeatMs = now;
  ibiNum = ibiPos = 0;
  bpm = 0;
}

void addIbi(int ibi) {
  ibiBuf[ibiPos] = ibi;
  ibiPos = (ibiPos + 1) % IBI_COUNT;
  if (ibiNum < IBI_COUNT) ibiNum++;

  long sum = 0;
  for (int i = 0; i < ibiNum; i++) sum += ibiBuf[i];
  bpm = (int)(60000L * ibiNum / sum);   // 平均IBI → BPM
}

// 1サンプルぶんの拍動検出
void detectBeat(int signal, uint32_t now) {
  uint32_t sinceBeat = now - lastBeatMs;

  // 拍動直後はノイズ（ディクロティックノッチ）を避けるため山/谷の更新を待つ
  if (sinceBeat > (uint32_t)(MIN_IBI_MS * 3 / 5)) {
    if (signal < thresh && signal < trough) trough = signal;
    if (signal > thresh && signal > peak)   peak = signal;
  }

  // しきい値を下から上に横切った → 拍動
  if (sinceBeat > MIN_IBI_MS && signal > thresh && !pulse) {
    pulse = true;
    if (hasLastBeat) {
      int ibi = (int)sinceBeat;
      if (ibi >= MIN_IBI_MS && ibi <= MAX_IBI_MS) addIbi(ibi);
    }
    hasLastBeat = true;
    lastBeatMs = now;
  }

  // しきい値を上から下に横切った → 振幅からしきい値を更新
  if (signal < thresh && pulse) {
    pulse = false;
    int amp = peak - trough;
    thresh = trough + amp / 2;
    peak = trough = thresh;
  }

  // 長時間拍動が無い → 指が離れたとみなしてリセット
  if (sinceBeat > NO_BEAT_RESET_MS) resetDetector(now);
}

void drawScreen() {
  // BPM（黒背景に上書き）
  M5.Display.fillRect(0, 70, 320, 100, TFT_BLACK);
  M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  if (bpm > 0) {
    M5.Display.setFont(&fonts::FreeSansBold24pt7b);
    M5.Display.setTextSize(2);
    M5.Display.drawString(String(bpm), 20, 80);
    M5.Display.setTextSize(1);
    M5.Display.setFont(&fonts::FreeSansBold12pt7b);
    M5.Display.drawString("BPM", 200, 130);
  } else {
    M5.Display.setTextSize(1);
    M5.Display.setFont(&fonts::FreeSansBold12pt7b);
    M5.Display.setTextColor(TFT_YELLOW, TFT_BLACK);
    M5.Display.drawString("Place your finger", 20, 100);
  }
}

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  Serial.begin(115200);

  M5.Display.setRotation(1);
  M5.Display.fillScreen(TFT_BLACK);
  M5.Display.setFont(&fonts::FreeSansBold12pt7b);
  M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Display.drawString("Heart Rate", 10, 10);

  analogReadResolution(12);   // 0〜4095
  resetDetector(millis());
  lastSampleUs = micros();
}

void loop() {
  M5.update();

  // 2msごとにサンプリング
  uint32_t nowUs = micros();
  if (nowUs - lastSampleUs >= SAMPLE_US) {
    lastSampleUs += SAMPLE_US;
    lastSignal = analogRead(SENSOR_PIN);
    detectBeat(lastSignal, millis());
  }

  uint32_t now = millis();

  // 画面：BPMの数値のみ
  if (now - lastDrawMs >= DRAW_INTERVAL_MS) {
    lastDrawMs = now;
    drawScreen();
  }

  // グラフを見たいときは VSCode の Teleplot で確認（10msごとに出力）
  if (now - lastPlotMs >= PLOT_INTERVAL_MS) {
    lastPlotMs = now;
    Serial.printf(">signal:%d\n>thresh:%d\n>bpm:%d\n", lastSignal, thresh, bpm);
  }
}

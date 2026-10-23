// CalibratedColorSensor.h
// TCS34725 を「白黒キャリブレーション＋ガンマ補正」付きで扱うクラス
//
// 使い方：
//   CalibratedColorSensor sensor;                 // 154ms / 4x（既定）
//   sensor.begin(Wire);                           // Wire.begin() は先に済ませておく
//   sensor.calibrateWhite();                      // 白い紙に当てて呼ぶ
//   sensor.calibrateBlack();                      // 黒い物に当てて呼ぶ
//   CalibratedColorSensor::Color c = sensor.read();
//   c.r, c.g, c.b      … 表示用 sRGB (0-255)
//   c.toRGB565()       … M5 の画面描画用
//
// キャリブレーション値は ESP32 の Preferences(NVS) に保存され、再起動後も使われる。
// ゲイン／積分時間が保存時と異なる場合、保存値は自動的に無効になる。

#pragma once
#include <Arduino.h>
#include <Wire.h>
#include <Preferences.h>
#include <Adafruit_TCS34725.h>

class CalibratedColorSensor {
public:
  struct Color {
    uint8_t  r = 0, g = 0, b = 0;          // 表示用 sRGB（ガンマ補正済み 0-255）
    float    linR = 0, linG = 0, linB = 0; // リニア値（白=1.0, 黒=0.0）
    float    rawR = 0, rawG = 0, rawB = 0; // 生カウント（平均値）
    float    clear = 0;                    // Clear チャンネル（平均値）
    bool     saturated = false;            // 飽和していたら true

    uint16_t toRGB565() const {
      return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
    }
    uint32_t toHex() const { return ((uint32_t)r << 16) | ((uint32_t)g << 8) | b; }
  };

  // integrationTime: TCS34725_INTEGRATIONTIME_xxMS / gain: TCS34725_GAIN_xX
  CalibratedColorSensor(uint8_t integrationTime = TCS34725_INTEGRATIONTIME_154MS,
                        tcs34725Gain_t gain     = TCS34725_GAIN_4X);

  // センサ初期化と保存済みキャリブレーションの読み込み。見つからなければ false
  bool begin(TwoWire &wire = Wire, const char *nvsNamespace = "tcs34725");

  // 平均化して読み取り、補正済みの色を返す（積分時間 × 平均回数 だけ時間がかかる）
  Color read();
  const Color &last() const { return _last; }

  // キャリブレーション（直前ではなく、呼んだ時点で新たに測定する）
  bool calibrateWhite();   // 飽和していたら記録せず false
  void calibrateBlack();
  void resetCalibration();
  bool isCalibrated() const { return _whiteSet; }

  // 設定
  void setAverageCount(uint8_t n) { _avg = n < 1 ? 1 : n; }
  void setGamma(float g)          { _gamma = g; }
  // ゲイン／積分時間の変更はキャリブレーションも無効化する
  void setGain(tcs34725Gain_t gain);
  void setIntegrationTime(uint8_t it);

  // LED 制御（基板の LED ピンを GPIO につないだ場合のみ有効）
  void attachLedPin(int pin);
  void setLed(bool on);

  Adafruit_TCS34725 &raw() { return _tcs; }   // ライブラリを直接触りたい場合

private:
  bool     readAveraged(float rgb[3], float &clear);
  uint8_t  toDisplay(float lin) const;
  uint16_t saturationLevel() const;
  void     save();
  void     load();

  Adafruit_TCS34725 _tcs;
  Preferences       _prefs;
  uint8_t           _it;
  tcs34725Gain_t    _gain;
  uint8_t           _avg   = 4;
  float             _gamma = 2.2f;
  int               _ledPin = -1;
  bool              _prefsOpen = false;

  float _white[3] = {1000, 1000, 1000};
  float _black[3] = {0, 0, 0};
  bool  _whiteSet = false;
  Color _last;
};

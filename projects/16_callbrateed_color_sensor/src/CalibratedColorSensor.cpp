// CalibratedColorSensor.cpp
#include "CalibratedColorSensor.h"

CalibratedColorSensor::CalibratedColorSensor(uint8_t integrationTime, tcs34725Gain_t gain)
    : _tcs(integrationTime, gain), _it(integrationTime), _gain(gain) {}

bool CalibratedColorSensor::begin(TwoWire &wire, const char *nvsNamespace) {
  if (!_tcs.begin(TCS34725_ADDRESS, &wire)) return false;
  _tcs.setIntegrationTime(_it);
  _tcs.setGain(_gain);
  _prefsOpen = _prefs.begin(nvsNamespace, false);
  load();
  return true;
}

// ---- 読み取り ----

// 積分時間から計算した最大カウント（の 90%）を飽和ラインとする
// 最大カウント = (256 - ATIME) * 1024（上限 65535）
uint16_t CalibratedColorSensor::saturationLevel() const {
  uint32_t maxCount = (uint32_t)(256 - _it) * 1024;
  if (maxCount > 65535) maxCount = 65535;
  return (uint16_t)(maxCount * 0.9f);
}

bool CalibratedColorSensor::readAveraged(float rgb[3], float &clear) {
  float sum[4] = {0, 0, 0, 0};
  bool saturated = false;
  uint16_t satLevel = saturationLevel();
  for (uint8_t i = 0; i < _avg; i++) {
    uint16_t r, g, b, c;
    _tcs.getRawData(&r, &g, &b, &c);   // 積分時間ぶん待ってから返る
    if (c >= satLevel) saturated = true;
    sum[0] += r; sum[1] += g; sum[2] += b; sum[3] += c;
  }
  for (int i = 0; i < 3; i++) rgb[i] = sum[i] / _avg;
  clear = sum[3] / _avg;
  return saturated;
}

uint8_t CalibratedColorSensor::toDisplay(float lin) const {
  lin = constrain(lin, 0.0f, 1.0f);
  return (uint8_t)(powf(lin, 1.0f / _gamma) * 255.0f + 0.5f);
}

CalibratedColorSensor::Color CalibratedColorSensor::read() {
  Color c;
  float raw[3];
  c.saturated = readAveraged(raw, c.clear);
  c.rawR = raw[0]; c.rawG = raw[1]; c.rawB = raw[2];

  float lin[3];
  for (int i = 0; i < 3; i++) {
    float span = _white[i] - _black[i];
    lin[i] = (span > 1.0f) ? (raw[i] - _black[i]) / span : 0.0f;
  }
  c.linR = lin[0]; c.linG = lin[1]; c.linB = lin[2];
  c.r = toDisplay(lin[0]);
  c.g = toDisplay(lin[1]);
  c.b = toDisplay(lin[2]);

  _last = c;
  return c;
}

// ---- キャリブレーション ----

bool CalibratedColorSensor::calibrateWhite() {
  float raw[3], clear;
  if (readAveraged(raw, clear)) return false;   // 飽和時は記録しない
  memcpy(_white, raw, sizeof(_white));
  _whiteSet = true;
  save();
  return true;
}

void CalibratedColorSensor::calibrateBlack() {
  float raw[3], clear;
  readAveraged(raw, clear);
  memcpy(_black, raw, sizeof(_black));
  save();
}

void CalibratedColorSensor::resetCalibration() {
  for (int i = 0; i < 3; i++) { _white[i] = 1000; _black[i] = 0; }
  _whiteSet = false;
  if (_prefsOpen) _prefs.clear();
}

// ---- 設定変更 ----

void CalibratedColorSensor::setGain(tcs34725Gain_t gain) {
  _gain = gain;
  _tcs.setGain(gain);
  resetCalibration();
}

void CalibratedColorSensor::setIntegrationTime(uint8_t it) {
  _it = it;
  _tcs.setIntegrationTime(it);
  resetCalibration();
}

// ---- LED ----

void CalibratedColorSensor::attachLedPin(int pin) {
  _ledPin = pin;
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
}

void CalibratedColorSensor::setLed(bool on) {
  if (_ledPin >= 0) digitalWrite(_ledPin, on ? HIGH : LOW);
}

// ---- 保存／読み込み ----

void CalibratedColorSensor::save() {
  if (!_prefsOpen) return;
  _prefs.putBytes("W", _white, sizeof(_white));
  _prefs.putBytes("K", _black, sizeof(_black));
  _prefs.putBool("Wset", _whiteSet);
  _prefs.putUChar("it", _it);
  _prefs.putUChar("gain", (uint8_t)_gain);
}

void CalibratedColorSensor::load() {
  if (!_prefsOpen) return;
  // 保存時と測定設定が違えば無効
  if (!_prefs.isKey("it") || !_prefs.isKey("gain")) return;
  if (_prefs.getUChar("it") != _it || _prefs.getUChar("gain") != (uint8_t)_gain) return;

  if (_prefs.getBytesLength("W") == sizeof(_white)) _prefs.getBytes("W", _white, sizeof(_white));
  if (_prefs.getBytesLength("K") == sizeof(_black)) _prefs.getBytes("K", _black, sizeof(_black));
  _whiteSet = _prefs.getBool("Wset", false);
}

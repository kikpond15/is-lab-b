#include <M5Unified.h>
#include <SD.h>

hw_timer_t* timer = nullptr;
volatile bool tick_flag = false;
int interval_seconds = 5;  // ★ ここを変えれば間隔変更（例：5秒ごと）
bool running = false;                // ★ スタート状態
File sdcard;

void saveColor(String t, uint32_t r, uint32_t g, uint32_t b);
void IRAM_ATTR onTimerISR() { tick_flag = true; /*ISRではフラグだけ*/ }

void setup(void) {
  auto cfg = M5.config();
  M5.begin(cfg);
  //RTC初期化
  m5::rtc_datetime_t t;
  t.date.year   = 2025;
  t.date.month  = 10;
  t.date.date   = 16;
  t.date.weekDay = 0;   // 任意（0=日曜日）
  t.time.hours   = 15;
  t.time.minutes = 0;
  t.time.seconds = 0;
  M5.Rtc.setDateTime(t);

  //割り込みタイマー初期化
  timer = timerBegin(0,80,true);
  timerAttachInterrupt(timer, &onTimerISR,true);
  timerAlarmWrite(timer, (uint64_t)interval_seconds * 1000000ULL,true);
  timerAlarmEnable(timer);

  //SDカード初期化
  SD.begin(4, SPI, 40000000); // SDカード初期化(CSピン4, SPIクロック40MHz)
  sdcard = SD.open("/m5_color.csv", FILE_WRITE); // 追記モードでオープン
  sdcard.println("time,r,g,b");// ヘッダー行書き込み
  sdcard.flush();

  randomSeed(micros());//乱数初期化
}

void loop(void) {

  M5.update();
  //RTCから日時取得
  auto dt = M5.Rtc.getDateTime();
  String DATE = String(dt.date.year) + "-" + String(dt.date.month) + "-" + String(dt.date.date);
  String TIME = String(dt.time.hours) + ":" + String(dt.time.minutes) + ":" + String(dt.time.seconds);

  // ★ Aボタンでスタート（タイマ有効化）
  if (M5.BtnA.wasPressed() && !running) {
    running = true;
    tick_flag = false;
    timerAlarmEnable(timer);
  }

  //割り込みタイマーでフラグが立ったら色を変えて保存
  if (tick_flag) {
      tick_flag = false;
      uint8_t R = random(256); // 0-255の乱数
      uint8_t G = random(256);
      uint8_t B = random(256);
      uint16_t COLOR = M5.Lcd.color565(R, G, B);
      M5.Lcd.fillRect(10, 100, 50, 50, COLOR);
      saveColor(TIME, R, G, B); // 色保存
  }

  M5.Display.drawString(DATE, 0, 0, 2);
  M5.Display.drawString(TIME, 0, 20, 2);
  if(running) M5.Display.drawString("Recoding", 0, 40, 2);
}

void saveColor(String t, uint32_t r, uint32_t g, uint32_t b) {
  // CSV形式で「時刻,R,G,B」書き込み(時刻は起動からのミリ秒)
  String line = t + ',' + String(r) + "," + String(g) + "," + String(b);
  if (sdcard)
  {
    sdcard.println(line); // 書き込み
    sdcard.flush();       // SDカードに保存
  }
}

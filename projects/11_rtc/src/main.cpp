#include <M5Unified.h>

void setup(void) {
  auto cfg = M5.config();
  M5.begin(cfg);
  m5::rtc_datetime_t t;
  t.date.year   = 2025;
  t.date.month  = 10;
  t.date.date   = 16;
  t.date.weekDay = 0;   // 任意（0=日曜日）
  t.time.hours   = 15;
  t.time.minutes = 0;
  t.time.seconds = 0;
  M5.Rtc.setDateTime(t);
  delay(1000);
}

void loop(void) {
  auto dt = M5.Rtc.getDateTime();
  M5.Display.drawString(String(dt.date.year)+" : " + String(dt.date.month)+" : "+ String(dt.date.date), 0, 0, 2);
  M5.Display.drawString(String(dt.time.hours)+" : " + String(dt.time.minutes)+" : "+ String(dt.time.seconds), 0, 20, 2);
  delay(500);
}

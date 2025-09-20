#include <Arduino.h>
#include <M5Unified.h>
#include "HCSR04.h"

const int trig_pin = G25;
const int echo_pin = G35;
HCSR04 hcsr04;

void setup() {
  m5::M5Unified::config_t cfg = M5.config();
  M5.begin(cfg);
  Serial.begin(115200);

  M5.Display.setTextSize(2);
  M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Display.clear();

#ifdef TARGET_M5BASIC
  M5.Display.println("Board: M5Stack Basic");
#elif defined(TARGET_M5CORE2)
  M5.Display.println("Board: M5Stack Core2");
#endif

  hcsr04.begin(trig_pin, echo_pin);
}

void loop() {
  M5.update();
  float dist = hcsr04.distance();
  
  char message[20];
  sprintf(message, "distance: %6.2fcm", dist);
  M5.Display.drawString(message, 0, 40);
  Serial.println(dist);
  delay(10);
}

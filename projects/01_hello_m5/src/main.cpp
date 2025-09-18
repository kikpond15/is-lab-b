#include <M5Unified.h>

void setup() {
  auto cfg = M5.config();      // M5Unifiedが機種差を吸収
  M5.begin(cfg);

  M5.Display.setRotation(1);   // 横向き
  M5.Display.setTextSize(2);
  M5.Display.clear();
  M5.Display.println("Hello, M5!");

#ifdef TARGET_M5BASIC
  M5.Display.println("Board: M5Stack Basic");
#elif defined(TARGET_M5CORE2)
  M5.Display.println("Board: M5Stack Core2");
#endif
}

void loop() {
  M5.update();                  // ボタン/タッチ等の状態更新

  // Basic: A/B/C ボタン、Core2: タッチ
  if (M5.BtnA.wasPressed())     M5.Display.println("BtnA pressed");
  if (M5.BtnB.wasPressed())     M5.Display.println("BtnB pressed");
  if (M5.BtnC.wasPressed())     M5.Display.println("BtnC pressed");

  // Core2のタッチ例（Basicでも安全に呼べる）
  auto t = M5.Touch.getDetail();
  if (t.isPressed()) {
    M5.Display.printf("Touch: (%d, %d)\n", t.x, t.y);
  }

  delay(10);
}


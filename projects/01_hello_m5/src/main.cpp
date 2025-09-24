#include <M5Unified.h>

void setup() {
  auto cfg = M5.config();      // M5Unifiedが機種差を吸収
  M5.begin(cfg);

  M5.Display.setRotation(1);   // 横向き
  M5.Display.setTextSize(2);
  M5.Display.clear();
  M5.Display.println("Hello, M5!");

}

void loop() {
  M5.update();                  // ボタン/タッチ等の状態更新
  
  String msg1 = "";
  // Core2のタッチ例（Basicでも安全に呼べる）
  auto t = M5.Touch.getDetail();
  if (t.isPressed()) {
    msg1 = "Touch: " + String(t.x) + "," + String(t.y);
  }
  M5.Display.drawString(msg1, 0, 40);  //画面に表示

  String msg2 = "";
  // Basic: A/B/C ボタン、Core2: タッチ
  if (M5.BtnA.wasPressed())     msg2 = "BtnA pressed";
  if (M5.BtnB.wasPressed())     msg2 = "BtnB pressed";
  if (M5.BtnC.wasPressed())     msg2 = "BtnC pressed";
  M5.Display.drawString(msg2, 0, 60);
  delay(10);
}


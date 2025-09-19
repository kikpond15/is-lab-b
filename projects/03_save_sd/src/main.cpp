#include <M5Unified.h>
#include <SD.h>
File sdcard;

// 関数のプロトタイプ宣言
void logButton(char btn);


void setup() {
  auto cfg = M5.config();      // M5Unifiedが機種差を吸収
  M5.begin(cfg);
  // Display初期化
  M5.Display.setRotation(1);   // 横向き
  M5.Display.setTextSize(2);
  M5.Display.clear();
  M5.Display.println("Save to SD");

  //SDカード初期化
  SD.begin(4, SPI, 40000000);
  sdcard = SD.open("/m5.csv", FILE_WRITE); // 追記モードでオープン
  sdcard.println("time, button");
  M5.Display.drawString("Logging Start", 0, 15);
  sdcard.flush();
}

void loop() {
  M5.update();  //状態更新
  // A/B/C ボタンの押下を検出
  if (M5.BtnA.wasPressed()){
    logButton('A');
  }
  if (M5.BtnB.wasPressed()){
    logButton('B');
  }
  if (M5.BtnC.wasPressed()){
    logButton('C');
  }
  delay(10);
}

void logButton(char btn) {
  // CSV形式で「時刻,ボタン」書き込み
  String line = String(millis()) + "," + String(btn);
  M5.Display.drawString(line, 0, 40);
  if (sdcard) {
    sdcard.println(line);// 書き込み
    sdcard.flush();// SDカードに保存
  }
}


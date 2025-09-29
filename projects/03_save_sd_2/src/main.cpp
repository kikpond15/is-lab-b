#include <M5Unified.h>
#include <SD.h>
File sdcard;
bool loggingState = false; // ログの状態を保持する変数
void logTouch(int x, int y); // 関数のプロトタイプ宣言

void setup() {
  auto cfg = M5.config();      // M5Unifiedが機種差を吸収
  M5.begin(cfg);
  // Display初期化
  M5.Display.setTextSize(2);
  M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Display.clear();
  M5.Display.println("Save to SD");

  SD.begin(4, SPI, 40000000); // SDカード初期化(CSピン4, SPIクロック40MHz)
  sdcard = SD.open("/m5.csv", FILE_WRITE); // 追記モードでオープン
  sdcard.println("time, x, y"); // ヘッダー行書き込み
  sdcard.flush();
}

void loop() {
  M5.update();
  //画面にタッチ位置を表示
  auto t = M5.Touch.getDetail();
  if (t.isPressed()){
    M5.Display.drawString("Touch: " + String(t.x) + "," + String(t.y) + "   ", 0, 50);
  }

  // ボタンAが押されたらログの開始/停止を切り替え
  if (M5.BtnA.wasPressed()){
    if(loggingState){
      loggingState = false;
      M5.Display.drawString("Logging Stop ", 0, 30);
    } else{
      loggingState = true;
      M5.Display.drawString("Logging Start", 0, 30);
    }
  }

  // ログの状態がONならタッチ位置をSDカードに保存
  if (loggingState == true) {
    if (t.isPressed()) logTouch(t.x, t.y);
  }
}

void logTouch(int x, int y) {
  String line = String(millis()) + "," + String(x) + "," + String(y);
  if (sdcard) {
    sdcard.println(line);// 書き込み
    sdcard.flush();// SDカードに保存
  }
}


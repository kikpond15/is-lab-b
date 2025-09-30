#include <M5Unified.h>
#include <SD.h>
File sdcard;
bool loggingState = false;
void saveSD(int hbt);

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  Serial.begin(115200);

  M5.Display.setTextSize(2);
  M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Display.clear();

  SD.begin(4, SPI, 40000000);  //SDカード初期化
  sdcard = SD.open("/m5.csv", FILE_WRITE); // 追記モードでオープン
  sdcard.println("time,heartbeat"); // ヘッダー行書き込み
  sdcard.flush();
}

void loop() {
  M5.update();
  int heartbeat = analogRead(36);

  M5.Display.drawString("heartbeat: " + String(heartbeat) , 0, 60);  
  Serial.printf(">heartbeat:%d\n", heartbeat); //Teleplot"でグラフ確認
  // Serial.printf("heartbeat:%d\n", heartbeat); //数値確認のみ  

  if (M5.BtnA.wasPressed()){  // ボタンAが押されたらログの開始/停止を切り替え
    if(loggingState){
      loggingState = false;
      M5.Display.drawString("Logging Stop ", 0, 30);
    } else{
      loggingState = true;
      M5.Display.drawString("Logging Start", 0, 30);
    }
  }

  // ログの状態がONならタッチ位置をSDカードに保存
  if (loggingState == true) saveSD(heartbeat);
  delay(10);
}

void saveSD(int hbt) {
  String line = String(millis()) + "," + String(hbt);
  if (sdcard) {
    sdcard.println(line);// 書き込み
    sdcard.flush();// SDカードに保存
  }
}

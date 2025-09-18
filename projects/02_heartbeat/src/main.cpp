//using heartbeat sensor with M5Stack Basic/Core2
//https://www.switch-science.com/products/1135

#include <M5Unified.h>

void setup() {
  auto cfg = M5.config();      // M5Unifiedが機種差を吸収
  M5.begin(cfg);
  Serial.begin(115200); //シリアルモニタ用

  M5.Display.setTextSize(2);
  M5.Display.setTextColor(TFT_WHITE, TFT_BLACK);
  M5.Display.clear();

#ifdef TARGET_M5BASIC
  M5.Display.println("Board: M5Stack Basic");
#elif defined(TARGET_M5CORE2)
  M5.Display.println("Board: M5Stack Core2");
#endif
}

void loop() {
  M5.update();
  int heatbeat = analogRead(36);//GPIO36(ADC1_CH0)を使用

  //画面に表示
  String msg = "heatbeat: " + String(heatbeat);
  M5.Display.drawString(msg, 0, 40);

  //シリアルモニタに表示(VSCodeの場合"Teleplot"で確認可能)
  Serial.printf("heatbeat:%d\n", heatbeat); //数値確認のみ
  Serial.printf(">heatbeat:%d\n", heatbeat); //グラフ確認
  delay(10);
  
}
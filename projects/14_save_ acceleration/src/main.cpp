#include <M5UnitLCD.h>
#include <M5UnitOLED.h>
#include <M5Unified.h>

void setup() {
  auto cfg = M5.config();
  M5.begin(cfg);
  
  // シリアル通信の初期化（115200bps）
  Serial.begin(115200);
  Serial.println("M5Stack IMU Serial Output Test");

  M5.Display.setTextSize(2);
  M5.Display.println("IMU Test");
  M5.Display.println("Data on Serial Monitor");

  // IMUセンサの初期化
  M5.Imu.begin();
}

void loop() {
  float accX = 0.0F;
  float accY = 0.0F;
  float accZ = 0.0F;
  float gyroX = 0.0F;
  float gyroY = 0.0F;
  float gyroZ = 0.0F;
  float temp = 0.0F;

  // センサからデータを読み取る
  M5.update();
  M5.Imu.getAccelData(&accX, &accY, &accZ);
  M5.Imu.getGyroData(&gyroX, &gyroY, &gyroZ);
  M5.Imu.getTemp(&temp);

  // ディスプレイに表示
  M5.Display.setCursor(0, 50);
  M5.Display.fillRect(0, 50, 320, 190, BLACK);
  
  M5.Display.printf("accel: X:%5.2f Y:%5.2f Z:%5.2f\n", accX, accY, accZ);
  M5.Display.printf("gyro : X:%5.2f Y:%5.2f Z:%5.2f\n", gyroX, gyroY, gyroZ);
  M5.Display.printf("temp : %5.2f\n", temp);

  // シリアルにデータを出力
  Serial.print("Accelerometer: X=");
  Serial.print(accX, 2);
  Serial.print(", Y=");
  Serial.print(accY, 2);
  Serial.print(", Z=");
  Serial.println(accZ, 2);
  
  Serial.print("Gyroscope: X=");
  Serial.print(gyroX, 2);
  Serial.print(", Y=");
  Serial.print(gyroY, 2);
  Serial.print(", Z=");
  Serial.println(gyroZ, 2);
  
  Serial.print("Temperature: ");
  Serial.println(temp, 2);
  
  Serial.println("------------------------");

  delay(500); // 出力間隔を500msに設定（データが見やすいように）
}
 
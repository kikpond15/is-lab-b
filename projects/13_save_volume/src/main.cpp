#include <M5Unified.h>
#include <SD.h>

static constexpr const size_t record_number = 256;
static constexpr const size_t record_length = 200;
static constexpr const size_t record_size = record_number * record_length;
static constexpr const size_t record_samplerate = 16000;
static int16_t prev_y[record_length];
static int16_t prev_h[record_length];
static size_t rec_record_idx = 2;
static size_t draw_record_idx = 0;
static int16_t *rec_data;

// 1秒間の平均音量計算用
static constexpr const size_t samples_per_second = record_samplerate / record_length; // 約80回
static int64_t volume_sum = 0;
static size_t sample_count = 0;

File sdcard;
bool loggingState = false; // ログの状態を保持する変数

void setup()
{
  auto cfg = M5.config();
  // cfg.external_speaker.hat_spk = true;     /// use external speaker (HAT SPK)
  // cfg.external_speaker.hat_spk2 = true;    /// use external speaker (HAT SPK2)
  // cfg.external_speaker.atomic_spk = true;  /// use external speaker (ATOMIC SPK)
  M5.begin(cfg);
  rec_data = (typeof(rec_data))heap_caps_malloc(record_size * sizeof(int16_t), MALLOC_CAP_8BIT);
  memset(rec_data, 0, record_size * sizeof(int16_t));
  M5.Speaker.setVolume(255);
  /// Since the microphone and speaker cannot be used at the same time, turn off the speaker here.
  M5.Speaker.end();
  M5.Mic.begin();

  SD.begin(4, SPI, 40000000); // SDカード初期化(CSピン4, SPIクロック40MHz)
  sdcard = SD.open("/m5.csv", FILE_WRITE); // 追記モードでオープン
  sdcard.println("time,value"); // ヘッダー行書き込み
  sdcard.flush();
}

void loop()
{
  M5.update();

  // ボタンAが押されたらログの開始/停止を切り替え
  if (M5.BtnA.wasPressed()){
    if(loggingState){
      loggingState = false;
    } else{
      loggingState = true;
    }
  }

  float average_volume = 0;

  if (M5.Mic.isEnabled())
  {
    static constexpr int shift = 6;
    auto data = &rec_data[rec_record_idx * record_length];
    if (M5.Mic.record(data, record_length, record_samplerate))
    {
      data = &rec_data[draw_record_idx * record_length];
      // 音量の計算(絶対値の平均)
      int64_t chunk_volume = 0;
      for (int i = 0; i < record_length; ++i)
      {
        chunk_volume += abs(data[i]);
      }
      volume_sum += chunk_volume;
      sample_count++;
      // 1秒経過したら平均音量を表示
      if (sample_count >= samples_per_second)
      {
        average_volume = (float)volume_sum / (sample_count * record_length);

        M5.Display.fillRect(0, 0, M5.Display.width(), 40, TFT_BLACK);
        M5.Display.setCursor(0, 0);
        M5.Display.setTextSize(2);
        M5.Display.printf(loggingState ? "Recording" : "Stop");
        M5.Display.setCursor(0, 20);
        M5.Display.printf("Vol: %.0f", average_volume);
        // リセット
        volume_sum = 0;
        sample_count = 0;
      }
      int32_t w = M5.Display.width();
      if (w > record_length - 1)
      {
        w = record_length - 1;
      }
      for (int32_t x = 0; x < w; ++x)
      {
        M5.Display.writeFastVLine(x, prev_y[x], prev_h[x], TFT_BLACK);
        int32_t y1 = (data[x] >> shift);
        int32_t y2 = (data[x + 1] >> shift);
        if (y1 > y2)
        {
          int32_t tmp = y1;
          y1 = y2;
          y2 = tmp;
        }
        int32_t y = (M5.Display.height() >> 1) + y1;
        int32_t h = (M5.Display.height() >> 1) + y2 + 1 - y;
        prev_y[x] = y;
        prev_h[x] = h;
        M5.Display.writeFastVLine(x, y, h, TFT_WHITE);
      }
      M5.Display.display();
      if (++draw_record_idx >= record_number)
      {
        draw_record_idx = 0;
      }
      if (++rec_record_idx >= record_number)
      {
        rec_record_idx = 0;
      }
    }
  }

  if (loggingState && sample_count == 0) {
    String line = String(millis()) + "," + String(average_volume);
    sdcard.println(line);
    sdcard.flush();
  }
}

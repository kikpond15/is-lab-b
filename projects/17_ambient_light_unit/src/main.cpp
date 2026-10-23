#include <M5Unified.h>
#include <M5_DLight.h>

M5Canvas canvas(&M5.Display);

M5_DLight sensor;
uint16_t lux;

void setup() {
    M5.begin();
    canvas.setTextDatum(MC_DATUM);
    canvas.setColorDepth(1);
    canvas.setFont(&fonts::Orbitron_Light_24);
    canvas.setTextSize(2);
    canvas.createSprite(M5.Display.width(), M5.Display.height());
    canvas.setPaletteColor(1, ORANGE);
    Serial.begin(115200);
    Serial.println("Sensor begin.....");
    sensor.begin();

    // CONTINUOUSLY_H_RESOLUTION_MODE
    // CONTINUOUSLY_H_RESOLUTION_MODE2
    // CONTINUOUSLY_L_RESOLUTION_MODE
    // ONE_TIME_H_RESOLUTION_MODE
    // ONE_TIME_H_RESOLUTION_MODE2
    // ONE_TIME_L_RESOLUTION_MODE
    sensor.setMode(CONTINUOUSLY_H_RESOLUTION_MODE);
}

char info[40];

void loop() {
    lux = sensor.getLUX();
    sprintf(info, "lux: %d", lux);
    canvas.fillSprite(BLACK);
    canvas.drawString(info, 160, 120);
    canvas.pushSprite(0, 0);
    Serial.println(info);
    delay(100);
}
#include "eye.h"

#define LEFT_DC_PIN 26
#define LEFT_CS_PIN 27
#define LEFT_RST_PIN 25

#define RIGHT_DC_PIN 17
#define RIGHT_CS_PIN 0
#define RIGHT_RST_PIN 16

#define SPI_FREQUENCY 30000000
#define NUM_EYES 2

#include <Adafruit_GFX.h>
#include <Adafruit_SSD1351.h>
#include <SPI.h>
Adafruit_SSD1351 eyeTFT[NUM_EYES] = {
  Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, new SPIClass(HSPI), LEFT_CS_PIN, LEFT_DC_PIN, LEFT_RST_PIN),
  Adafruit_SSD1351(SCREEN_WIDTH, SCREEN_HEIGHT, new SPIClass(VSPI), RIGHT_CS_PIN, RIGHT_DC_PIN, RIGHT_RST_PIN)
};
GFXcanvas16 eyeCanvas(SCREEN_WIDTH, SCREEN_HEIGHT);
Eye eye(&eyeCanvas);

void setup(void) {
  Serial.begin(115200);
  Serial.print("Begin");
  for (int i = 0; i < NUM_EYES; i++) {
    eyeTFT[i].begin(SPI_FREQUENCY);
  }
  Serial.println("  Done");
}

void loop() {
  // FPS calculation
  unsigned long int curTime = millis();
  static unsigned long int fpsCounter = 0;
  static unsigned long int fpsStartTimeMillis = millis();
  fpsCounter++;
  unsigned long timeDiff = curTime - fpsStartTimeMillis;
  if (timeDiff > 1000) {
    Serial.print("FPS ");
    Serial.println(fpsCounter * (1000.0 / timeDiff));
    fpsStartTimeMillis = curTime;
    fpsCounter = 0;
  }

  // Render to canvas and transfer canvas to displays
  eye.renderToCanvas();
  for (int i = 0; i < NUM_EYES; i++) {
    eyeTFT[i].drawRGBBitmap(0, 0, eyeCanvas.getBuffer(), eyeCanvas.width(), eyeCanvas.height());
  }
}

#ifndef __EYE_H__
#define __EYE_H__
#include <Adafruit_GFX.h>
#include "data.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 128

// Color definitions
#define BLACK 0x0000
#define WHITE 0xFFFF
#define YELLOW 0xFEC0

#define PUPIL_BASE_SIZE 32

class Eye {
private:
  GFXcanvas16* canvas;
  double eyeX;
  double eyeY;
  bool isLooking;
  unsigned long int lookingTimeMillis;
  unsigned long int lookingTimeStartMillis;
  double eyeXDir;
  double eyeYDir;
  unsigned long int moveTimeMillis;
  unsigned long int moveStartTimeMillis;
  double pupilRadius;
  double finalPupilRadius;
  double startPupilRadius;
  bool shouldChangePupilRadius;
  bool isBlinking;
  unsigned long int blinkStartMillis;
  unsigned long int blinkDurationMillis;
  unsigned long int nextBlinkInMillis;
  double eyeLidOffset;

  void updateEyePosition();
  void updatePupilSize();
  void updateEyelidOffset();
  void drawRGBBitmapOptimized(int x, int y, const uint16_t data[], int width, int height);
  unsigned long int getRandomMoveTimeMillis();
  unsigned long int getRandomBlinkDurationMillis();
  unsigned long int getRandomNextBlinkMillis();
  double getRandomEyeDir();
  double getRandomPupilSize();
public:
  Eye(GFXcanvas16* canvas);
  void renderToCanvas();
};
#endif
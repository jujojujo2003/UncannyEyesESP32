#include "eye.h"

unsigned long int Eye::getRandomMoveTimeMillis() {
  return 500 + esp_random() % 2000;
}

// Returns a random number between 1 & -1
double Eye::getRandomEyeDir() {
  int number = 1000 - esp_random() % 2001;
  return number * 1.0 / 1000;
}

double Eye::getRandomPupilSize() {
  int number = 100 - esp_random() % 30;
  return (number * 1.0 / 100) * PUPIL_BASE_SIZE;
}

unsigned long int Eye::getRandomBlinkDurationMillis() {
  return 300 + esp_random() % 500;
}

unsigned long int Eye::getRandomNextBlinkMillis() {
  return 2800 + esp_random() % 3000;
}

Eye::Eye(GFXcanvas16* canvas) {
  this->canvas = canvas;
  this->eyeX = SCREEN_WIDTH / 2;
  this->eyeY = SCREEN_HEIGHT / 2;
  this->isLooking = false;
  this->moveStartTimeMillis = millis();
  this->moveTimeMillis = this->getRandomMoveTimeMillis();
  this->eyeXDir = this->getRandomEyeDir();
  this->eyeYDir = this->getRandomEyeDir();
  this->pupilRadius = PUPIL_BASE_SIZE;
  this->shouldChangePupilRadius = false;
  this->isBlinking = false;
  this->blinkStartMillis = millis();
  this->blinkDurationMillis = this->getRandomBlinkDurationMillis();
  this->nextBlinkInMillis = this->getRandomNextBlinkMillis();
  this->eyeLidOffset = 0;
}



void Eye::updateEyePosition() {
  if (this->isLooking) {
    this->updatePupilSize();
    unsigned long int curTimeMillis = millis();
    // If done looking, start moving eyes
    if ((curTimeMillis - this->lookingTimeStartMillis) >= this->lookingTimeMillis) {
      this->isLooking = false;
      this->moveStartTimeMillis = millis();
      this->moveTimeMillis = this->getRandomMoveTimeMillis();
      this->eyeXDir = this->getRandomEyeDir();
      this->eyeYDir = this->getRandomEyeDir();
    }
  } else {
    unsigned long int curTimeMillis = millis();
    // Sine smoothing 0->1->0
    double rad = (((curTimeMillis - this->moveStartTimeMillis) * 1.0 / this->moveTimeMillis)) * 3.14159;
    this->eyeX = this->eyeX + this->eyeXDir * sin(rad);
    this->eyeY = this->eyeY + this->eyeYDir * sin(rad);

    bool shouldStartLooking = false;

    // If eye hits limits, start looking
    if (this->eyeX > (SCREEN_WIDTH / 2.0 + MAX_IRIS_MOVEMENT_IN_X)) {
      this->eyeX = SCREEN_WIDTH / 2.0 + MAX_IRIS_MOVEMENT_IN_X;
      shouldStartLooking = true;
    } else if (this->eyeX < (SCREEN_WIDTH / 2.0 - MAX_IRIS_MOVEMENT_IN_X)) {
      this->eyeX = SCREEN_WIDTH / 2.0 - MAX_IRIS_MOVEMENT_IN_X;
      shouldStartLooking = true;
    }

    if (this->eyeY > (SCREEN_HEIGHT / 2.0 + MAX_IRIS_MOVEMENT_IN_Y)) {
      this->eyeY = SCREEN_HEIGHT / 2.0 + MAX_IRIS_MOVEMENT_IN_Y;
      shouldStartLooking = true;
    } else if (this->eyeY < (SCREEN_HEIGHT / 2.0 - MAX_IRIS_MOVEMENT_IN_Y)) {
      this->eyeY = SCREEN_HEIGHT / 2.0 - MAX_IRIS_MOVEMENT_IN_Y;
      shouldStartLooking = true;
    }

    // If move time has elapsed, start looking
    if ((curTimeMillis - this->moveStartTimeMillis) >= this->moveTimeMillis) {
      shouldStartLooking = true;
    }

    if (shouldStartLooking) {
      this->isLooking = true;
      this->lookingTimeStartMillis = millis();
      this->lookingTimeMillis = this->getRandomMoveTimeMillis();
      this->shouldChangePupilRadius = this->lookingTimeMillis <= 1000;
      this->finalPupilRadius = this->getRandomPupilSize();
      this->startPupilRadius = this->pupilRadius;
    }
  }
}

void Eye::updatePupilSize() {
  if (!this->shouldChangePupilRadius) {
    return;
  }
  unsigned long int curTimeMillis = millis();
  // Sine smoothing 0->1
  double rad = (((curTimeMillis - this->lookingTimeStartMillis) * 1.0 / this->lookingTimeMillis)) * 3.14159 * 0.5;
  double percent = min(max(sin(rad), 0.0), 1.0);
  this->pupilRadius = this->startPupilRadius * (1.0 - percent) + this->finalPupilRadius * percent;
}

void Eye::updateEyelidOffset() {
  unsigned long int curTimeMillis = millis();
  if (this->isBlinking) {
    // If blink time is up , reset next blink time
    if ((curTimeMillis - this->blinkStartMillis) > this->blinkDurationMillis) {
      this->isBlinking = false;
      this->nextBlinkInMillis = this->getRandomNextBlinkMillis();
      this->eyeLidOffset = 0;
    } else {
      // Sine smoothing 0->1->0
      double rad = (((curTimeMillis - this->blinkStartMillis) * 1.0 / this->blinkDurationMillis)) * 3.14159;
      this->eyeLidOffset = sin(rad) * 40;
    }
  } else if ((this->blinkStartMillis + this->blinkDurationMillis + this->nextBlinkInMillis) < curTimeMillis) {
    this->isBlinking = true;
    this->blinkStartMillis = curTimeMillis;
    this->blinkDurationMillis = this->getRandomBlinkDurationMillis();
  }
}

void Eye::drawRGBBitmapOptimized(int x, int y, const uint16_t data[], int width, int height) {
  for (int i = y; i <= SCREEN_HEIGHT && i <= height; i++) {
    if (i >= 0) {
      int numberOfPixelsToCopyInCurrentRow = min(SCREEN_WIDTH, x + width);
      memcpy(this->canvas->getBuffer() + (i * SCREEN_WIDTH), data + ((i - y) * width) - x, numberOfPixelsToCopyInCurrentRow * sizeof(uint16_t));
    }
  }
}

void Eye::renderToCanvas() {
  this->updateEyePosition();
  this->updateEyelidOffset();
  this->canvas->fillScreen(BLACK);
  this->drawRGBBitmapOptimized((int)this->eyeX - IRIS_CENTER_IN_SCHLERA_X, (int)this->eyeY - IRIS_CENTER_IN_SCHLERA_Y, SCHLERA, SCHLERA_WIDTH, SCHLERA_HEIGHT);

  // Draw pupil
  this->canvas->fillCircle(this->eyeX, this->eyeY, this->pupilRadius, BLACK);
  // Draw pupil highlights
  this->canvas->fillCircle(this->eyeX - (SCREEN_WIDTH / 2.0 - this->eyeX) / 10, this->eyeY - (SCREEN_HEIGHT / 2.0 - this->eyeY) / 10, this->pupilRadius / 1.5, YELLOW);

  // Draw eyelids
  this->canvas->fillRect(0, 0, SCREEN_WIDTH, this->eyeLidOffset, BLACK);
  this->canvas->fillRect(0, SCREEN_HEIGHT - this->eyeLidOffset, SCREEN_WIDTH, this->eyeLidOffset + 1, BLACK);
  this->canvas->drawBitmap(0, -10 + this->eyeLidOffset, UPPER_EYELID, UPPER_EYELID_WIDTH, UPPER_EYELID_HEIGHT, BLACK);
  this->canvas->drawBitmap(0, 10 - this->eyeLidOffset, LOWER_EYELID, LOWER_EYELID_WIDTH, LOWER_EYELID_HEIGHT, BLACK);
}
#include "qlock.h"

void Fade(Adafruit_NeoPixel* strip, float color) {
    for (int i = 0; i < LED_COUNT; i++) {
        strip->setPixelColor(i, strip->Color(0, 0, 0, color));
    }

    strip->show();
}
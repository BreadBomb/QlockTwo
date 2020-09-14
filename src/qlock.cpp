#define QLOCK_DEFINE_GLOBAL_VARS
#include "qlock.h"
#include <Arduino.h>

Qlock::Qlock() 
{
}

void Qlock::setup() {
    Serial.begin(9600);
    strip.begin();

    setupRTC();
}

void Qlock::loop() {
    strip.clear();

    RtcDateTime time = getTime();

    int hour = h24t12(time.Hour());
    int minute = time.Minute();

    showWord(WORD::ES);
    showWord(WORD::IST);

    showHour(time);
    showMinute(time);

    loopClock();

    strip.show();
}


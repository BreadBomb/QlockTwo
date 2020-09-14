#include "qlock.h"

void setup() {
    Qlock::instance().setup();
}

void loop() {
    Qlock::instance().loop();
}
#include "value_animation.h"
#include "Arduino.h"

ValueAnimation::ValueAnimation(int from, int to, int duration, bool startImediatly) {
    this->from = from;
    this->to = to;
    this->duration = duration;
    this->begin = startImediatly;
    this->startTime = -1;

    if (begin) {
        startTime = millis();
    }
}

void ValueAnimation::start() {
    begin = true;
    startTime = -1;
}

bool ValueAnimation::isStarted() {
    return this->begin;
}

float ValueAnimation::handle() {
    if (!begin) return (double)from;

    if (startTime == -1) startTime = millis();


    float currentValue = from + (float)(to - from) * ((float)millis() - (float)startTime) / (float)duration;

    if (currentValue >= to) {
        begin = false;
        return to;
    }

    return currentValue;
}

int ValueAnimation::handleRounded() {
    if (!begin) return from;

    if (startTime == -1) startTime = millis();

    if (from == to) {
        begin = false;
        return to;
    }

    int _to = max(from, to);
    int _from = min(from, to);

    float currentValue = _from + (float)(_to - _from) * ((float)millis() - (float)startTime) / (float)duration;

    if (currentValue >= _to) {
        begin = false;
        return to;
    }

    if (from == _from) {
        return (int)round(currentValue);
    } else {
        return (int)round(_from + (_to - currentValue));
    }

}
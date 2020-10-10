#include "qlock.h"

void(*callbackPointer)(String);

void SetSettingsChangedCallback(void(*func)(String)) {
    callbackPointer = func;
}

bool IsOn() {
    return isOn;
}

void SetOn(bool value) {
    isOn = value;
    callbackPointer("power");
}

uint32_t GetColor() {
    uint32_t color;
    EEPROM.get(0, color);
    return color;
}

void SetColor(uint32_t color) {
    ledColor = color;
    EEPROM.put(0, ledColor);
    callbackPointer("color");
    EEPROM.commit();
}

bool GetAutoTime() {
    bool autoT;
    EEPROM.get(32, autoT);
    return autoT;    
}

void SetAutoTime(bool value) {
    autoTime = value;
    EEPROM.put(32, value);
    EEPROM.commit();
}

void loadSettings() {
    ledColor = GetColor();
    autoTime = GetAutoTime();
}


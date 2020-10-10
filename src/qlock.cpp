#define QLOCK_DEFINE_GLOBAL_VARS
#include "qlock.h"
#include <Arduino.h>

#include <ESP8266httpUpdate.h>
#include <ArduinoOTA.h>
#include <value_animation.h>


AsyncWebServer s(80);
DNSServer dns;

Qlock::Qlock() 
{
}

ValueAnimation startAnimation = ValueAnimation(0, 255, 2000, false);
ValueAnimation powerOnAnimation = ValueAnimation(0, 255, 200, false);
ValueAnimation powerOffAnimation = ValueAnimation(255, 0, 200, false);
ValueAnimation brightnessAnimation = ValueAnimation(0, 255, 0, false);

void Qlock::setup() {
    Serial.begin(9600);
    strip.begin();

    server = &s;

    Serial.println("==== QLOCKTWO ====");
    Serial.println("Version: " + VERSION);

    setupRTC();
    Serial.println("RTC running");
    EEPROM.begin(1024);
    Serial.println("EEPROM running");
    beginServer();
    Serial.println("Server started");
    ArduinoOTA.begin(true);
    Serial.println("OTA started");
    EEPROM.begin(512);
    Serial.println("EEPROM initialized");

    loadSettings();

    SetSettingsChangedCallback([&](String setting) {
        if (setting == "power") {
            Serial.println("Power settins changed");
            if (isOn) {
                powerOnAnimation.start();
            } else {
                powerOffAnimation.start();
            }
        } else if (setting == "color") {
            
        }
    });
}

int brightnessTimer = 0;
int brightnessCounter = 0;
int brightnessMeasured = 0;

void handleBrightness() {
    int sensor_brightness = analogRead(A0);

    double brightness_percentage = 1.0 - (double)sensor_brightness / 1024.0;

    int brightness = (int)round(MIN_BRIGHTNESS + ((MAX_BRIGHTNESS-MIN_BRIGHTNESS) * brightness_percentage));

    brightnessMeasured += brightness;
    brightnessCounter++;

    if(millis() - brightnessTimer >= 1500 && isOn && !powerOnAnimation.isStarted()) {
        brightnessTimer = millis();

        brightnessAnimation = ValueAnimation(strip.getBrightness(), brightnessMeasured / brightnessCounter, 500, true);

        brightnessCounter = 0;
        brightnessMeasured = 0;
    }
}

void Qlock::loop() {
    ArduinoOTA.handle();
    handleBrightness();

    if (millis() > 2000 && !wifiConfigured) {
        AsyncWiFiManager wifiManager(server, &dns);

        WiFi.hostname("qlocktwo");

        if (!wifiManager.autoConnect("QlockTwo")) {
            return;
        }

        wifiConfigured = true;

        Serial.println("Wifi connected");
    }

    loopServer();

    if (restartESP) {
        Serial.println("restart");
        ESP.restart();
    }

    if (startUpdate) {
        Serial.println("Start spiffs update");
        HTTPUpdateResult result = ESPhttpUpdate.updateSpiffs("http://qlocktwo.azurewebsites.net/spiffs.bin");
        if (result != HTTP_UPDATE_OK) {
            Serial.println(ESPhttpUpdate.getLastErrorString());
        }
        Serial.println("Spiffs update done!");
        Serial.println("Start firmware update");
        ESPhttpUpdate.rebootOnUpdate(true);
        HTTPUpdateResult result1 = ESPhttpUpdate.update("http://qlocktwo.azurewebsites.net/firmware.bin");
        if (result1 != HTTP_UPDATE_OK) {
            Serial.println(ESPhttpUpdate.getLastErrorString());
        }
        Serial.println("Firmware update done!");
        startUpdate = false;
    }

    strip.clear();

    RtcDateTime time = getTime();

    int hour = h24t12(time.Hour());
    int minute = time.Minute();

    showWord(WORD::ES);
    showWord(WORD::IST);

    showHour(time);
    showMinute(time);

    loopClock();

    if (!startAnimation.isStarted()) {
        startAnimation.start();
    }

    if (brightnessAnimation.isStarted()) {
        strip.setBrightness(brightnessAnimation.handleRounded());
    }

    if (!wifiConfigured) {
        float num = startAnimation.handle();
        Fade(&strip, num);
    }

    if (powerOnAnimation.isStarted()) {
        strip.setBrightness(powerOnAnimation.handleRounded());        
    }

    if (powerOffAnimation.isStarted()) {
        strip.setBrightness(powerOffAnimation.handleRounded());
    }

    strip.show();

    delay(10);
}


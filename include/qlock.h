#ifndef QLOCK_H
#define QLOCK_H

#include <Arduino.h>
#include <EEPROM.h>

// LEDs
#include "Adafruit_NeoPixel.h"

// RTC
#include <Wire.h>
#include <RtcDS3231.h>

//Server
#include <WiFiUdp.h>
#include <ESP8266WiFi.h>
#include <ESPAsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ESPAsyncWiFiManager.h> 
#include <NTPClient.h>
#include <FS.h>

#define countof(a) (sizeof(a) / sizeof(a[0]))

#ifndef QLOCK_DEFINE_GLOBAL_VARS
    # define QLOCK_GLOBAL extern
    # define _INIT(x)
    # define _INIT_N(x)
#else
    # define QLOCK_GLOBAL
    # define _INIT(x) = x

    //needed to ignore commas in array definitions
    #define UNPACK( ... ) __VA_ARGS__
    # define _INIT_N(x) UNPACK x
#endif

enum WORD {
    ES,
    IST,
    FUENF1,
    ZEHN1,
    ZWANZIG,
    DREI1,
    VIERTEL,
    NACH,
    VOR,
    HALB,
    ZWOELF,
    ZWEI,
    EIN,
    EINS,
    SIEBEN,
    DREI2,
    FUENF2,
    ELF,
    NEUN,
    VIER,
    ACHT,
    ZEHN2,
    SECHS,
    UHR
};

#define LED_COUNT 114

#define MIN_BRIGHTNESS 10
#define MAX_BRIGHTNESS 255

QLOCK_GLOBAL Adafruit_NeoPixel strip _INIT(Adafruit_NeoPixel(LED_COUNT, D4, NEO_GRBW + NEO_KHZ800));

QLOCK_GLOBAL AsyncWebServer * server;

QLOCK_GLOBAL String VERSION _INIT("0.0.6");

QLOCK_GLOBAL bool startUpdate _INIT(false);

QLOCK_GLOBAL bool restartESP _INIT(false);

QLOCK_GLOBAL bool wifiConfigured _INIT(false);

QLOCK_GLOBAL uint32_t freeHeap _INIT(0);


//Settings
QLOCK_GLOBAL bool isOn _INIT(true);
QLOCK_GLOBAL uint32_t ledColor _INIT(strip.Color(0, 0, 0, 255));
QLOCK_GLOBAL bool autoTime _INIT(true);

#include "fcn_declare.h"

class Qlock {
    public:
        Qlock();
        static Qlock& instance()
        {
            static Qlock instance;
            return instance;
        }

        void setup();
        void loop();
};
#endif
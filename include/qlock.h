#ifndef QLOCK_H
#define QLOCK_H

#include <Arduino.h>

// LEDs
#include "Adafruit_NeoPixel.h"

// RTC
#include <Wire.h>
#include <RtcDS3231.h>

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

QLOCK_GLOBAL Adafruit_NeoPixel strip _INIT(Adafruit_NeoPixel(114, D4, NEO_GRBW + NEO_KHZ800));

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
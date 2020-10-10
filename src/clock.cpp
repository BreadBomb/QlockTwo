#include "qlock.h"
#include <stdarg.h>

void _powerLedFromTo(int from, int to, uint32_t color) {
    for (int i = from; i <= to; i++) {
        strip.setPixelColor(i, color);
    }
}

void showWord(WORD word) {
    uint32_t color = ledColor;

    switch (word) {
        case WORD::ES:
            _powerLedFromTo(111, 112, color);
            break;
        case WORD::IST:
            _powerLedFromTo(107, 109, color);
            break;
        case WORD::FUENF1:
            _powerLedFromTo(102, 105, color);
            break;
        case WORD::ZEHN1:
            _powerLedFromTo(90, 93, color);
            break;
        case WORD::ZWANZIG:
            _powerLedFromTo(94, 100, color);
            break;
        case WORD::DREI1:
            _powerLedFromTo(86, 89, color);
            break;
        case WORD::VIERTEL:
            _powerLedFromTo(79, 85, color);
            break;
        case WORD::NACH:
            _powerLedFromTo(70, 73, color);
            break;
        case WORD::VOR:
            _powerLedFromTo(74, 76, color);
            break;  
        case WORD::HALB:
            _powerLedFromTo(64, 67, color);
            break;
        case WORD::ZWOELF:
            _powerLedFromTo(58, 62, color);
            break;
        case WORD::ZWEI:
            _powerLedFromTo(46, 49, color);
            break;
        case WORD::EIN:
            _powerLedFromTo(48, 50, color);
            break;
        case WORD::EINS:
            _powerLedFromTo(48, 51, color);
            break;
        case WORD::SIEBEN:
            _powerLedFromTo(51, 56, color);
            break;
        case WORD::DREI2:
            _powerLedFromTo(41, 44, color);
            break;
        case WORD::FUENF2:
            _powerLedFromTo(35, 38, color);
            break;
        case WORD::ELF:
            _powerLedFromTo(24, 26, color);
            break;
        case WORD::NEUN:
        _powerLedFromTo(27, 30, color);
            break;
        case WORD::VIER:
            _powerLedFromTo(31, 34, color);
            break;
        case WORD::ACHT:
            _powerLedFromTo(19, 22, color);
            break;
        case WORD::ZEHN2:
            _powerLedFromTo(15, 18, color);
            break;
        case WORD::SECHS:
            _powerLedFromTo(2, 6, color);
            break;
        case WORD::UHR:
            _powerLedFromTo(9, 11, color);
            break;
    }
}

void showHour(RtcDateTime time) {
    int hour = h24t12(time.Hour());
    int minute = time.Minute();

    if (hour == 0 && minute < 25 || hour == 11 && minute >= 25 || hour == 12 && minute < 25) {
        showWord(WORD::ZWOELF);
    } else if (hour == 1 && minute < 5) {
        showWord(WORD::EIN);
    } else if (hour == 1 && minute >= 5 && minute < 25 || hour == 0 && minute >= 25 || hour == 12 && minute >= 25) {
        showWord(WORD::EINS);
    } else if (hour == 2 && minute < 25 || hour == 1 && minute >= 25) {
        showWord(WORD::ZWEI);
    } else if (hour == 3 && minute < 25 || hour == 2 && minute >= 25) {
        showWord(WORD::DREI2);
    } else if (hour == 4 && minute < 25 || hour == 3 && minute >= 25) {
        showWord(WORD::VIER);
    } else if (hour == 5 && minute < 25 || hour == 4 && minute >= 25) {
        showWord(WORD::FUENF2);
    } else if (hour == 6 && minute < 25 || hour == 5 && minute >= 25) {
        showWord(WORD::SECHS);
    } else if (hour == 7 && minute < 25 || hour == 6 && minute >= 25) {
        showWord(WORD::SIEBEN);
    } else if (hour == 8 && minute < 25 || hour == 7 && minute >= 25) {
        showWord(WORD::ACHT);
    } else if (hour == 9 && minute < 25 || hour == 8 && minute >= 25) {
        showWord(WORD::NEUN);
    } else if (hour == 10 && minute < 25 || hour == 9 && minute >= 25) {
        showWord(WORD::ZEHN2);
    } else if (hour == 11 && minute < 25 || hour == 10 && minute >= 25) {
        showWord(WORD::ELF);
    }
}

void showPoint(int minute) {
    uint32_t color = ledColor;

    int pointCount = minute % 5;
    if (pointCount >= 1) {
        strip.setPixelColor(113, color);
    }
    if (pointCount >= 2) {
        strip.setPixelColor(101, color);
    }
    if (pointCount >= 3) {
        strip.setPixelColor(12, color);
    }
    if (pointCount >= 4) {
        strip.setPixelColor(0, color);
    }
}

void showMinute(RtcDateTime time) {
    int minute = time.Minute();

    if (minute < 5) {
        showWord(WORD::UHR);
    } else if (minute >= 5 && minute < 10) {
        showWord(WORD::FUENF1);
        showWord(WORD::NACH);
    } else if (minute >= 10 && minute < 15) {
        showWord(WORD::ZEHN1);
        showWord(WORD::NACH);
    } else if (minute >= 15 && minute < 20 ) {
        showWord(WORD::VIERTEL);
        showWord(WORD::NACH);
    } else if (minute >= 20 && minute < 25) {
        showWord(WORD::ZWANZIG);
        showWord(WORD::NACH);
    } else if (minute >= 25 && minute < 30) {
        showWord(WORD::FUENF1);
        showWord(WORD::VOR);
        showWord(WORD::HALB);
    } else if (minute >= 30 && minute < 35) {
        showWord(WORD::HALB);
    } else if (minute >= 35 && minute < 40) {
        showWord(WORD::FUENF1);
        showWord(WORD::NACH);
        showWord(WORD::HALB);
    } else if (minute >= 40 && minute < 45) {
        showWord(WORD::ZWANZIG);
        showWord(WORD::VOR);
    } else if (minute >= 45 && minute < 50) {
        showWord(WORD::VIERTEL);
        showWord(WORD::VOR);
    } else if (minute >= 50 && minute < 55) {
        showWord(WORD::ZEHN1);
        showWord(WORD::VOR);
    } else if (minute >= 55) {
        showWord(WORD::FUENF1);
        showWord(WORD::VOR);
    }

    showPoint(minute);
}

void loopClock() {

}
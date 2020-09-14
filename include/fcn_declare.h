//clock.cpp
void showWord(WORD word);
void showHour(RtcDateTime time);
void showMinute(RtcDateTime time);
void loopClock();

//rtc.cpp
void setupRTC();
RtcDateTime getTime();
int h24t12(int hour);
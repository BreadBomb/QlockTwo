//animations.cpp
void Fade(Adafruit_NeoPixel* strip, float color);

//server.cpp
void beginServer();
void loopServer();

//clock.cpp
void showWord(WORD word);
void showHour(RtcDateTime time);
void showMinute(RtcDateTime time);
void loopClock();

//rtc.cpp
void setupRTC();
RtcDateTime getTime();
int h24t12(int hour);
void setRtcTime(RtcDateTime rdt);

//settings.cpp
void loadSettings();
bool GetAutoTime();
void SetAutoTime(bool value);
void SetSettingsChangedCallback(void(*func)(String));
bool IsOn();
void SetOn(bool value);
uint32_t GetColor();
void SetColor(uint32_t color);
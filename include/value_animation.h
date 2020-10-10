#ifndef VALUE_ANIMATION_H
#define VALUE_ANIMATION_H
class ValueAnimation {
    public:
    ValueAnimation(int from, int to, int duration, bool startImediatly = true);
    void start();
    int handleRounded();
    float handle();
    bool isStarted();

    private:
    int begin;
    int from;
    int to;
    int duration = 1000;
    int startTime;
};
#endif
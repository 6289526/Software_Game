#pragma once

// #include <chrono>
#include <time.h>

class Timer
{
private:
    // std::chrono::system_clock::time_point _Prev;
    clock_t _Prev;
    clock_t _Now;

    int _CurrentTime = 0; // 現在時間
public:
    Timer();
    ~Timer();
    double GetDeltaTime();
    void UpdateFrame();

    void SetCurrentTime(int time) { _CurrentTime = time; }
    int GetCurrentTime() { return _CurrentTime; }
};

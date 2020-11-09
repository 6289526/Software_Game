#pragma once

// #include <chrono>
#include <time.h>

class Timer
{
private:
    // std::chrono::system_clock::time_point _Prev;
    clock_t _Prev;
    clock_t _Now;
public:
    Timer();
    ~Timer();
    double GetDeltaTime();
    void UpdateFrame();
};

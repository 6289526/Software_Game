#pragma once

// #include <chrono>
#include <time.h>

class Timer
{
private:
    // std::chrono::system_clock::time_point _Prev;
    clock_t _Prev;
public:
    Timer();
    ~Timer();
    double GetDeltaTime();
    void UpdateFrame();
};

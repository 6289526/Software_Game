#pragma once

#include <chrono>

class Timer
{
private:
    std::chrono::system_clock::time_point _Prev;
public:
    Timer();
    ~Timer();
    double GetDeltaTime();
    void UpdateFrame();
};

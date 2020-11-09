#pragma once

#include <chrono>

class Timer
{
private:
    static std::chrono::system_clock::time_point _Prev;
public:
    double GetDeltaTime();
    void UpdateFrame();
    Timer();
    ~Timer();
};

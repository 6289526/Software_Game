/**
 * @file client_time.hpp
 * @brief 
 * タイマーモジュールの定義
 */

#pragma once

#include <time.h>

class Timer
{
private:
    clock_t _Prev;
    clock_t _Now;

    int _CurrentTime = 0; // 現在時間
public:
    Timer();
    double GetDeltaTime();
    void UpdateFrame();

    inline void SetCurrentTime(int time) { _CurrentTime = time; }
    inline int GetCurrentTime() { return _CurrentTime; }
};

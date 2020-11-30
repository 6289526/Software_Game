#include "client_time.hpp"
// using namespace std::chrono;

Timer::Timer()
{
    _Prev = _Now = clock();
    UpdateFrame();
}

Timer::~Timer() { }

double Timer::GetDeltaTime(){
    // return static_cast<double>(duration_cast<microseconds>(system_clock::now() - _Prev).count() / 1000.0);
    return static_cast<double>(_Now - _Prev) / CLOCKS_PER_SEC;
}

void Timer::UpdateFrame(){
    // _Prev = system_clock::now();
    _Prev = _Now;
    _Now = clock();
    
    /*clock_t start = clock();

    // 何かの処理

    clock_t end = clock();

    const double time = static_cast<double>(end - start) / CLOCKS_PER_SEC * 1000.0;*/
}

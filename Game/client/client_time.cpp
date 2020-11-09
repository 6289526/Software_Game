#include "client_time.hpp"
using namespace std::chrono;

Timer::Timer()
{
    _Prev = system_clock::now();
}

Timer::~Timer() { }

double Timer::GetDeltaTime(){
    return static_cast<double>(duration_cast<microseconds>(_Prev - system_clock::now()).count() / 1000.0);
}

void Timer::UpdateFrame(){
    _Prev = system_clock::now();
}

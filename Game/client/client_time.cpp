/**
 * @file client_time.cpp
 * @brief 
 * タイマーモジュールの処理実装
 */

#include "client_time.hpp"

/**
 * @brief 
 * 現在時間をセットする
 */
Timer::Timer()
{
    _Prev = _Now = clock();
    UpdateFrame();
}

/**
 * @brief 
 * 前フレームとの差を返す
 * @return double 時間の差(sec)
 */
double Timer::GetDeltaTime(){
    return static_cast<double>(_Now - _Prev) / CLOCKS_PER_SEC;
}

/**
 * @brief 
 * フレームを更新する
 */
void Timer::UpdateFrame(){
    _Prev = _Now;
    _Now = clock();
}

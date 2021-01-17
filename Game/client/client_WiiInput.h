/**
 * @file client_WiiInput.h
 * @brief wiiリモコンの入力管理
 * @version 0.1
 * @date 2021-01-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#pragma once
#include "client_input.h"
// c言語のライブラリなので言語リンケージの指定が必要
extern "C"{  
#include "wiimote.h"	// Wiiリモコンを用いるために必要なヘッダファイル
#include "wiimote_api.h"	// Wiiリモコンを用いるために必要なヘッダファイル
}
class WiiInput : public InputModuleBase
{
private:
    // Wiiリモコンを用いるための構造体を宣言（初期化）
    wiimote_t _wiimote;	// Wiiリモコンの状態格納用
    wiimote_report_t _report;	// レポートタイプ用
public:
    
    WiiInput(char Address[17]);
    ~WiiInput();
    virtual void UpdateInput();
};

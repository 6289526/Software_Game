#include "client_WiiInput.h"

WiiInput::WiiInput(char Address[17]) : _wiimote(WIIMOTE_INIT), _report(WIIMOTE_REPORT_INIT) // レポートタイプ用
{
    fprintf(stderr, "%s\n", Address);
    // Wiiリモコンの接続（１つのみ）
    if (wiimote_connect(&_wiimote, Address) < 0)
    { // コマンド引数に指定したWiiリモコン識別情報を渡して接続
        printf("unable to open wiimote: %s\n", wiimote_get_error());
    }
    _wiimote.led.one = 1; // WiiリモコンのLEDの一番左を点灯させる（接続を知らせるために）
    // センサからのデータを受け付けるモードに変更
    _wiimote.mode.acc = 1;
}

WiiInput::~WiiInput()
{
}

void WiiInput::UpdateInput()
{
    if (wiimote_is_open(&_wiimote))
    {
        InputType t = _Input;
        // _Input = t;
        // Wiiリモコンの状態を取得・更新する
        if (wiimote_update(&_wiimote) < 0)
        {
            wiimote_disconnect(&_wiimote);
        }

        // ***** Wiiのキー（ボタン）ごとに処理 *****
        // HOMEボタンが押された時(終了処理)
        if (_wiimote.keys.home)
        {
            wiimote_disconnect(&_wiimote); // Wiiリモコンとの接続を解除
            _Input.End = true;
        }

        // Bボタンが押された時
        if (_wiimote.keys.b)
        {
            fprintf(stderr, "B\n");
            _Input.Put = true;
        }
        else
        {

            _putFlag = false;
        }

        // 移動関連
        // 1ボタンが押された時
        if (_wiimote.keys.one)
        {
            _Input.Jump = true;
        }
        else
        {
            _jumpFlag = false;
        }

        // 2ボタンが押された時
        if (_wiimote.keys.two)
        {
            _Input.Forward = true;
        }
        else
        {
            _Input.Forward = false;
        }

        if (_wiimote.keys.up)
        { // 上
            _Input.Left = true;
        }
        else
        {
            _Input.Left = false;
        }

        if (_wiimote.keys.down)
        { // 下
            _Input.Right = true;
        }
        else
        {
            _Input.Right = false;
        }
    }
}

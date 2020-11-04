/*
 *  ファイル名  : client_common.h
 *  機能    : クライアント用のマクロ、構造体等の定義
 */
#pragma once
/*----------include 開始----------*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <errno.h>
#include "../header/constants.h"
#include <SDL2/SDL.h>
#include <iostream>
//
/*----------include 終了----------*/

/*----------define 開始-----------*/

/*----------define 終了-----------*/

/*----------構造体宣言 開始-----------*/
// /*ネットワークモジュール用のクライアントの情報*/
// typedef struct {
//     int connect;              /*サーバーに接続しているか*/
//     int sock;                 /*使用するソケット*/
//     struct sockaddr_in addr;  /*ソケットの設定*/
// } NetworkData;
// /*システムモジュールが持ってるプレイヤーデータ*/
// typedef struct{
//     char name[MAX_LEN_NAME];    /*名前*/
//     FloatCube pos;              /*位置*/
//     int rank;                   /*順位*/
//     int goal;                   /*ゴールしたか*/
// } PlayerData;

/*----------構造体宣言 終了-----------*/

/*----------グローバル変数 開始-----------*/
/*変数*/


/* net.c */
extern void SetupServer(int, u_short);
extern void TerminateServer();
extern int ControlRequests();
extern void RunCommand(int, char);

using namespace std;

/* sys.cpp */
template <typename T>
class Pointer{
public:
    Pointer(int size) : m_point(new T[size]) { m_size = size; }
    Pointer(const Pointer<T>& p) {
        Copy(p);
    }
    ~Pointer() { delete[] m_point; }
    const T operator[](int n) const { return m_point[n]; }
    T& operator[](int n)      { return m_point[n]; }
    Pointer& operator=(const Pointer& p) {
        delete[] m_point;
        Copy(p);
    }
private:
    void Copy(const Pointer& p) {
        m_size = p.m_size;
        m_point = new T[m_size];
        for (int i = 0; i < m_size; ++i) {
            m_point[i] = p[i];
        }
    }


public:
    int m_size;
private:
    T* m_point;
};

const PlayerData* GetPlayerData();
       // キャラとマップの当たり判定
       // y : 当たり判定をとる座標ｙの補正(キャラの足元座標からの差)
       // accuracy : 当たり判定の精度(座標軸ごとの判定する座標数)
       //            例：２で２・２の４点　３で３・３の９点を判定する
       BlockType Collision_CM(int chara_ID, int y = 0, int accuracy = 2);
       bool Collision_BB(); // ブロックを置けるなら true
extern void GetClientName(int id,char clientName[MAX_LEN_NAME]);
       void Goal(int chara_ID);    // ゴールの処理
extern void MovePosition(int chara_ID); // キャラを移動させる
extern void PutBlock(); // ブロックを置けるなら置く
extern int AllGoal(); // 全員ゴールしていれば１
extern void SetVec(int chara_ID, Vector3& vec); // キャラの速度ベクトルをセット
extern void SetPlaceData(int chara_ID, PlaceData& data); //
extern void SendAllPos(int client_num); // クライアント全員に全員の座標を送る

extern void SetDirection(int chara_ID, float direction); // システムにクライアントの角度を渡す
/*-----------グローバル変数 終了----------*/

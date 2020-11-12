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
#include "../header/map.hpp"
#include "server_map.hpp"
//
/*----------include 終了----------*/

/*----------define 開始-----------*/

/*----------define 終了-----------*/

/*----------構造体宣言 開始-----------*/


/*----------構造体宣言 終了-----------*/

/*----------グローバル変数 開始-----------*/
/*変数*/
extern ServerMap Map;

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
    Pointer(int size);
    Pointer(const Pointer<T>& p);
    ~Pointer();
    const T operator[](int n) const;
    T& operator[](int n);
    Pointer& operator=(const Pointer& p);
    int Size();

private:
    void Copy(const Pointer& p);

private:
    T* m_point;
    int m_size;
};


template <class T>
Pointer<T>::Pointer(int size)
    : m_point(new T[size])
{
    m_size = size;
}

template <class T>
Pointer<T>::Pointer(const Pointer<T>& p)
{
    Copy(p);
}

template <class T>
Pointer<T>::~Pointer()
{
    delete[] m_point;
}

template <class T>
const T Pointer<T>::operator[](int n) const
{
    if (n < 0 || m_size <= n) {
        throw "Pointer [] : 範囲外\n";
    }
    return m_point[n];
}

template <class T>
T& Pointer<T>::operator[](int n)
{
    if (n < 0 || m_size <= n) {
        throw "Pointer [] : 範囲外\n";
    }
    return m_point[n];
}

template <class T>
Pointer<T>& Pointer<T>::operator=(const Pointer& p)
{
    delete[] m_point;
    Copy(p);
}

template <class T>
int Pointer<T>::Size()
{
    return m_size;
}

template <class T>
void Pointer<T>::Copy(const Pointer& p)
{
    m_size = p.m_size;
    m_point = new T[m_size];
    for (int i = 0; i < m_size; ++i) {
        m_point[i] = p[i];
    }
}

// 当たり判定で跳ね返す方向
enum Collision_Dire {
    Non,
    _Goal,
    Right,
    Back,
    Left,
    Front
};

struct Collision {
    Collision_Dire dire; // キャラを跳ね返す方向
    int power; // キャラを跳ね返す力
};

const PlayerData* GetPlayerData();

void SetNumClients(int n); // クライアント人数セット

void SetClientName(int id, char* name);

void InitSys(char* file); // システム初期化

void InitPlayerData(); // プレイヤーデータ初期化処理

void EndSys(); // システム終了処理

// キャラとブロックの当たり判定
// ｙ ： 基準面の高さの補正
// accuracy : 当たり判定の精度の調整　１以上 かつ キャラの幅・高さ以下の値
static Collision Collision_CB_Side(const int chara_ID, const int y = 0, const int accuracy = PLAYER_W);

static BlockType Collision_CB_Under(int chara_ID, int y, int accuracy);

static bool Collision_BB(); // ブロックを置けるなら true

static void Goal(int chara_ID);    // ゴールの処理

void MovePosition(int chara_ID); // キャラを移動させる

void PutBlock(int chara_ID); // ブロックを置けるなら置く

int AllGoal(); // 全員ゴールしていれば１

void SetVec(int chara_ID, Vector3& vec); // キャラの速度ベクトルをセット

void SetPlaceData(PlaceData& data); // 配置したいブロックの場所をセット

void SendAllPos(int client_num); // クライアント全員に全員の座標を送る

void SetDirection(int chara_ID, float direction); // システムにクライアントの角度を渡す
/*-----------グローバル変数 終了----------*/

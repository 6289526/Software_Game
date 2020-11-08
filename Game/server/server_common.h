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
    if (m_size <= n) {
        throw "Pointer [] : 範囲外\n";
    }
    return m_point[n];
}

template <class T>
T& Pointer<T>::operator[](int n)
{
    if (m_size <= n) {
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

const PlayerData* GetPlayerData();

void InitSys(char* file); // システム初期化

// キャラとブロックの当たり判定
// y : 当たり判定をとる座標ｙの補正(キャラの足元座標からの差)
// accuracy : 当たり判定の精度(座標軸ごとの判定する座標数)
//            例：２で２・２の４点　３で３・３の９点を判定する
static BlockType Collision_CB(int chara_ID, int y = 0, int accuracy = 2);

static bool Collision_BB(); // ブロックを置けるなら true

void GetClientName(int id,char clientName[MAX_LEN_NAME]);

static void Goal(int chara_ID);    // ゴールの処理

void MovePosition(int chara_ID); // キャラを移動させる

void PutBlock(int chara_ID); // ブロックを置けるなら置く

int AllGoal(); // 全員ゴールしていれば１

void SetVec(int chara_ID, Vector3& vec); // キャラの速度ベクトルをセット

void SetPlaceData(PlaceData& data); // 配置したいブロックの場所をセット

void SendAllPos(int client_num); // クライアント全員に全員の座標を送る

void SetDirection(int chara_ID, float direction); // システムにクライアントの角度を渡す
/*-----------グローバル変数 終了----------*/

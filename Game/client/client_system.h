/*
 *  ファイル名  : client_input.h
 *  機能    : ClientSystemにある変数と関数の定義
 */

#pragma once
#include "client_common.h"

void SetNumClients(int n); // クライアント人数セット

// 名前のセット
// id: クライアントのID
// clientName:クライアントの名前
void SetClientName(int id, char* name);

void InitPlayerData(); // プレイヤーデータ初期化処理

void EndSys(); // システム終了処理

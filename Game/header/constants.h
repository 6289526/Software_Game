/*
 *  ファイル名	: constants.h
 *  機能	: マクロ、構造体の定義
 */
#pragma once
/*----------include 開始----------*/
#include <netinet/in.h>
#include <stdio.h>

/*----------include 終了----------*/

/*----------define 開始-----------*/
#define DEFAULT_PORT 51000 	//デフォルトのポート番号
#define MAX_LEN_NAME 32 	//名前の最大文字数
#define MAX_NUMCLIENTS 8	//最大参加人数
#define MAX_LEN_BUFFER 256 	//メッセージの最大文字数
#define MAX_LEN_ADDR 32 	//ソケットの設定のサイズ
#define BROADCAST -1 		//全員に送るとき
#define PLAYER_NUM 2 		// クライアントの人数


#define PLAYER_X 40 		// クライアントの
#define PLAYER_Y 30 		// クライアントの
#define PLAYER_Z 40 		// クライアントの初期座標
#define PLAYER_W 10 		// クライアントの
#define PLAYER_D 10			// クライアントの
#define PLAYER_H 20 		// クライアントの大きさ

/*クライアントが使用するコマンド*/
#define MOVE_COMMAND 'M'
#define PUT_COMMAND 'P'
#define DO_NOT_PUT_COMMAND 'D'
#define GOAL_COMMAND 'G'
#define ERROR_COMMAND 'E'
#define FINISH_COMMAND 'F'
#define QUIT_COMMAND 'Q'
#define TIMER_COMMAND 'I'
#define TERMINATE_COMMAND 'T'
/*----------define 終了-----------*/

/*----------構造体宣言 開始-----------*/
/*座標情報*/
typedef struct
{
	int x; //x座標
	int y; //y座標
	int z; //z座標
} IntPosition;
typedef IntPosition Vector3Int;

typedef struct
{
	float x; //x座標
	float y; //y座標
	float z; //z座標
} FloatPosition;
typedef FloatPosition Vector3;

typedef struct {
	int x;	// x座標
	int y;	// y座標
} Vector2Int;

typedef struct {
	float x;// x座標
	float y;// y座標
} Vector2;

enum {
	NO_ZERO = 1,
	ZERO = 0
};

// 速度がゼロかを教えてくれる
typedef struct
{
	bool x;
	bool y;
	bool z;
} VelocityFlag;

typedef struct
{
	int x; //x座標
	int y; //y座標
	int z; //z座標
	int w; //x方向の長さ
	int h; //y方向の長さ
	int d; //z方向の長さ
} IntCube;

typedef struct
{
	float x; //x座標
	float y; //y座標
	float z; //z座標
	float w; //x方向の長さ
	float h; //y方向の長さ
	float d; //z方向の長さ
} FloatCube;

/*ネットワークモジュール用のクライアントの情報*/
typedef struct
{
	int connect;             /*サーバーに接続しているか*/
	int sock;                /*使用するソケット*/
	struct sockaddr_in addr; /*ソケットの設定*/
} NetworkData;

//プレイヤーの向き
typedef struct{
	float horizontal;
	float vertical;
}Direction;

/*クライアントの情報*/
typedef struct
{
	char name[MAX_LEN_NAME]; // 名前
	FloatCube pos;           // 場所
	Vector3 velocity;        // 速度ベクトル
	Direction direction;         // 向き
	int rank;                // 順位
	bool goal;               // ゴールしているか
	bool onGame; 			 // ゲームプレイ中かどうか
} PlayerData;

typedef struct
{
	int id;
	char name[MAX_LEN_NAME]; // 名前
} PlayerName;

typedef enum
{
	NonBlock = 0, // ブロックなし
	NomalBlock = 1, // wブロック
	MetalBlock = 2, // 設置用ブロック
	GoalBlock = -1, // ゴールブロック

} BlockType;

// 設置するオブジェクトのデータ
typedef struct
{
	BlockType object; // 置く物のID
	IntPosition pos; /*場所*/
} PlaceData;

/*----------構造体宣言 終了-----------*/

template <typename T> class Pointer {

public:
  Pointer(int size);
  Pointer(const Pointer<T> &p);
  ~Pointer();
  const T operator[](int n) const;
  T &operator[](int n);
  Pointer &operator=(const Pointer &p);
  int Size();
  const T *Get() const;

private:
  void Copy(const Pointer &p);

private:
  T *m_point;
  int m_size;
};

template <class T> inline Pointer<T>::Pointer(int size) : m_point(new T[size]) {
  m_size = size;
}

template <class T> Pointer<T>::Pointer(const Pointer<T> &p) { Copy(p); }

template <class T> inline Pointer<T>::~Pointer() { delete[] m_point; }

template <class T> inline const T Pointer<T>::operator[](int n) const {
  if (n < 0 || m_size <= n) {
    throw "Pointer [] : 範囲外\n";
  }
  return m_point[n];
}

template <class T> inline T &Pointer<T>::operator[](int n) {
  if (n < 0 || m_size <= n) {
    throw "Pointer [] : 範囲外\n";
  }
  return m_point[n];
}

template <class T> Pointer<T> &Pointer<T>::operator=(const Pointer &p) {
  delete[] m_point;
  Copy(p);
}

template <class T> inline int Pointer<T>::Size() { return m_size; }

template <class T> const T *Pointer<T>::Get() const { return m_point; }

template <class T> void Pointer<T>::Copy(const Pointer &p) {
  m_size = p.m_size;
  m_point = new T[m_size];
  for (int i = 0; i < m_size; ++i) {
    m_point[i] = p[i];
  }
}

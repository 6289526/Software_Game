#pragma once

#include "constants.h"
#include <vector>

using namespace std;

//マップサイズ
#define MAP_SIZE_W 40
#define MAP_SIZE_H 40
#define MAP_SIZE_D 40

//マップブロックのサイズ
#define MAP_MAGNIFICATION 30.0f
//設置オブジェクトのサイズ
// #define OBJECT_MAGNIFICATION 15.0f

class MapData{
    protected:
        int _TerrainData[MAP_SIZE_W][MAP_SIZE_H][MAP_SIZE_D] = {{{0}}};  //マップデータ
        // vector<PlaceData> _ObjectDatas; //オブジェクトデータ
        int _MapW = MAP_SIZE_W, _MapH = MAP_SIZE_H, _MapD = MAP_SIZE_D;  //マップデータ

    public:
        //マップデータの取得
        const int(* GetTerrainData())[MAP_SIZE_H][MAP_SIZE_D]{
            return _TerrainData;
        }
        //オブジェクトデータの取得
        // const vector<PlaceData> GetObjectData(){
        //     return _ObjectDatas;
        // }
        //マップサイズの取得
        int GetMapW(){
            return _MapW;
        }
        int GetMapH(){
            return _MapH;
        }
        int GetMapD(){
            return _MapD;
        }
        //オブジェクトデータの追加
        // void PushBackObject(PlaceData *objectData){
        //     _ObjectDatas.push_back(*objectData);
        // }
        void SetObjectData(PlaceData *objectData){
            _TerrainData[(int)(objectData->pos.x / MAP_MAGNIFICATION)][(int)(objectData->pos.y / MAP_MAGNIFICATION)][(int)(objectData->pos.z / MAP_MAGNIFICATION)] = objectData->object;
        }
};

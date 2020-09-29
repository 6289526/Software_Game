#pragma once

#include "constants.h"

#define MAP_SIZE_W 20
#define MAP_SIZE_H 20
#define OBJECT_NUM 5

class MapData{
    protected:
        int _TerrainData[MAP_SIZE_W][MAP_SIZE_H] = {0};
        PlaceData _ObjectDatas[OBJECT_NUM];
        int _MapW = MAP_SIZE_W, _MapH = MAP_SIZE_H;
        int _ObjectNum = 0;

    public:
        const int(* GetTerrainData())[MAP_SIZE_H]{
            return _TerrainData;
        };
        const PlaceData* GetObjectData(){
            return _ObjectDatas;
        };
        int GetMapW(){
            return _MapW;
        }
        int GetMapH(){
            return _MapH;
        }
        int GetObjectNum(){
            return _ObjectNum;
        }
};
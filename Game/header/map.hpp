#pragma once

#include "constants.h"
#include <vector>

using namespace std;

#define MAP_SIZE_W 20
#define MAP_SIZE_H 20
#define MAP_SIZE_D 20
#define OBJECT_NUM 5

class MapData{
    protected:
        int _TerrainData[MAP_SIZE_W][MAP_SIZE_H][MAP_SIZE_D] = {0};
        vector<PlaceData> _ObjectDatas;
        int _MapW = MAP_SIZE_W, _MapH = MAP_SIZE_H, _MapD = MAP_SIZE_D;

    public:
        const int(* GetTerrainData())[MAP_SIZE_H][MAP_SIZE_D]{
            return _TerrainData;
        }
        const vector<PlaceData> GetObjectData(){
            return _ObjectDatas;
        }
        int GetMapW(){
            return _MapW;
        }
        int GetMapH(){
            return _MapH;
        }
        void PushBackObject(PlaceData *objectData){
            _ObjectDatas.push_back(*objectData);
        }
};
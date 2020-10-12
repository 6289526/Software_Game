#include "server_map.hpp"

//filename:マップデータのファイル名
void ServerMap::LoadMapData(char* fileName){
    FILE *fp = fopen(fileName,"r");
    if (fp == NULL) {
        printf("failed to open map data file.");
    }
    //幅、奥行の読み取り
    fscanf(fp,"%d,%d\n",&_MapW,&_MapD);
    int baceTerrainData = 0;
    _MapD = 0;
    //マップデータの設定
    for(int depth = 0; depth < _MapD; depth++){
        for(int width = 0; width < _MapW; width++){
            fscanf(fp,"%d,",&baceTerrainData);
            for(int hight = 0; hight < baceTerrainData; hight++){
                _TerrainData[width][hight][depth] = 1;
            }
            if(_MapH < baceTerrainData) _MapH = baceTerrainData;
        }
    }
}
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
    //マップデータの設定
    for(int depth = 0; depth < _MapD; depth++){
        for(int width = 0; width < _MapW; width++){
            _TerrainData[width * 2][0][depth * 2] = 1;
            fscanf(fp,"%d,",&baceTerrainData);
            for(int hight = 1; hight <= baceTerrainData; hight++){
                _TerrainData[width * 2][hight * 2][depth * 2] = 1;
                _TerrainData[(width * 2) - 1][(hight * 2) - 1][(depth * 2) - 1] = 1;
            }
            if(_MapH < baceTerrainData) _MapH = baceTerrainData;
        }
    }
    _MapW *= 2;
    _MapH *= 2;
    _MapW *= 2;
}
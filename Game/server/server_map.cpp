#include "server_map.hpp"

//filename:マップデータのファイル名
void ServerMap::LoadMapData(char* fileName){
    FILE *fp = fopen(fileName,"r");
    if (fp == NULL) {
        printf("failed to open map data file.\n");
    }
    //幅、奥行の読み取り
    fscanf(fp,"%d %d\n",&_MapW,&_MapD);
    int baceTerrainData = 0;
    //マップデータの設定
    for(int depth = 0; depth < _MapD; depth++){
        for(int width = 0; width < _MapW; width++){
            _TerrainData[width][0][depth] = 1;
            fscanf(fp,"%d ",&baceTerrainData);
            if(baceTerrainData < 0) _TerrainData[width][0][depth] = 0;
            for(int hight = 1; hight <= baceTerrainData; hight++){
                _TerrainData[width][hight][depth] = NomalBlock;
                //fprintf(stderr,"(%d,%d,%d) = 1\n",width,hight,depth);
            }
            if(_MapH < baceTerrainData) _MapH = baceTerrainData;
        }
    }
    fclose(fp);
    //仮
    _TerrainData[3][0][4] = GoalBlock;
    _TerrainData[39][0][39] = 1;
    _TerrainData[39][1][39] = 1;
}

#include "server_map.hpp"

//filename:マップデータのファイル名
void ServerMap::LoadMapData(char *fileName)
{
    FILE *fp = fopen(fileName, "r");
    if (fp == NULL)
    {
        printf("failed to open map data file.\n");
    }
    //幅、奥行の読み取り
    fscanf(fp, "%d %d\n", &_MapW, &_MapD);
    if(_MapD > MAP_SIZE_D - 2) _MapD = MAP_SIZE_D - 2;
    int baceTerrainData = 0;
    //マップデータの設定
    for (int depth = 0; depth < _MapD; depth++)
    {
        for (int width = 0; width < _MapW; width++)
        {
            _TerrainData[width][0][depth] = 1;
            fscanf(fp, "%d ", &baceTerrainData);
            if(baceTerrainData >= MAP_SIZE_H){
                baceTerrainData = MAP_SIZE_H - 1;
            }
            if (baceTerrainData < 0){
                _TerrainData[width][0][depth] = NonBlock;
            }
            for (int hight = 1; hight <= baceTerrainData; hight++)
            {
                _TerrainData[width][hight][depth] = NomalBlock;
                //fprintf(stderr,"(%d,%d,%d) = 1\n",width,hight,depth);
            }
            if (_MapH < baceTerrainData)
                _MapH = baceTerrainData;
        }
    }
    for (int width = 0; width < _MapW; width++){
        for (int hight = 1; hight <= _MapH; hight++){
            _TerrainData[width][hight][_MapD - 1] = GoalBlock;
        }
    }
    fclose(fp);
}

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
    int baceTerrainData = 0;
    //マップデータの設定
    for (int depth = 0; depth < _MapD; depth++)
    {
        for (int width = 0; width < _MapW; width++)
        {
            _TerrainData[width][0][depth] = 1;
            if(depth == (_MapD - 1)){
                _TerrainData[width][0][depth] = GoalBlock;
            }
            fscanf(fp, "%d ", &baceTerrainData);
            if (baceTerrainData < 0){
                _TerrainData[width][0][depth] = NonBlock;
            }
            for (int hight = 1; hight <= baceTerrainData; hight++)
            {
                _TerrainData[width][hight][depth] = NomalBlock;
                //fprintf(stderr,"(%d,%d,%d) = 1\n",width,hight,depth);
                if(depth == (_MapD - 1) && hight == baceTerrainData){
                    _TerrainData[width][hight][depth] = GoalBlock;
                }
            }
            if (_MapH < baceTerrainData)
                _MapH = baceTerrainData;
        }
    }
    fclose(fp);
}

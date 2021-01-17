
#include "client_map.hpp"

/**
 * @fn
 * @brief マップデータを設定する
 * @param mapW マップのWサイズ
 * @param mapH マップのHサイズ
 * @param mapD マップのDサイズ
 * @param terrainData マップデータ
 */
void ClientMap::SetMapData(int mapW,int mapH, int mapD, int terrainData[MAP_SIZE_W][MAP_SIZE_H][MAP_SIZE_D]){
    _MapW = mapW;
    _MapH = mapH;
    _MapD = mapD;
    for(int i = 0; i < MAP_SIZE_W; i++){
        for(int j = 0; j < MAP_SIZE_H; j++){
            for(int k = 0; k < MAP_SIZE_D; k++){
                _TerrainData[i][j][k] = terrainData[i][j][k];
            }
        }
    }
}

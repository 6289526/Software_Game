#include "client_map.hpp"

void ClientMap::SetMapData(int mapW,int mapH, int mapD, int terrainData[MAP_SIZE_W][MAP_SIZE_H][MAP_SIZE_D]){
    _MapW = mapW;
    _MapH = mapH;
    _MapD = mapD;
    for(int i = 0; i < MAP_SIZE_H; i++){
        for(int j = 0; j < MAP_SIZE_H; j++){
            for(int k = 0; k < MAP_SIZE_D; k++){
                _TerrainData[i][j][k] = terrainData[i][j][k];
            }
        }
    }
}
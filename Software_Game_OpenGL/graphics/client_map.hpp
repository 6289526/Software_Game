#include "../header/map.hpp"

class ClientMap : public MapData{
    private:
    public:
        //マップデータの初期設定
        void SetMapData(int mapW,int mapH, int mapD, int terrainData[MAP_SIZE_W][MAP_SIZE_H][MAP_SIZE_D]);
};
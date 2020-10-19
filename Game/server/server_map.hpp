#include "../header/map.hpp"

class ServerMap : public MapData{
    private:
    public:
        //マップデータの初期設定
        void LoadMapData(char* fileName);
};
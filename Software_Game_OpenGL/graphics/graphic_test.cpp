#include"graphic_test.hpp"

PlayerData PData[PLAYER_NUM] = {
    {"a",{20.0,20.0,20.0,7.0,20.0,7.0},1,0,0},
    {"a",{20.0,20.0,20.0,10.0,10.0,10.0},1,0,0}
};
int myid = 0;
const double PI = 3.141592;


const PlayerData* GetPlayerData(){
    return PData;
}

void SetPlayerPos(int id, double x, double y, double z, double dir){
    PData[id].pos.x += x;
    PData[id].pos.y += y;
    PData[id].pos.z += z;
    dir *= PI;
    if(dir >= 2* PI)dir -= 2*PI;
    PData[id].direction = dir;
    //printf("%f, %f, %f\n",PData[id].pos.x, PData[id].pos.y, PData[id].pos.z);
}

int GetMyID(){
    return myid;
}
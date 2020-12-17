#include "graphic_2dui.hpp"

const double PI = 3.141592;
const double UI_MAGNIFICATION = 0.15/900 * Wd_Height;

void TimeImg::Set(TTF_Font *font){
    char numbers_string[10][2] = {"0","1", "2", "3", "4", "5", "6", "7", "8", "9"};
    char s[2] = "s";
    SDL_Color color {255,255,255,255};
    for(int i = 0; i < 10; i++){
        nums[i].LoadText(font,numbers_string[i],color);
    }
    str_s.LoadText(font, s, color);
}

void TimeImg::DrawTime(int time, FloatRect dst){
    if(time > 999) time = 999;
    if(time < 0) time = 0;
    int tmpnum[3] = {time / 100, (time/10)%10, time%10};
    FloatRect tmpdst, tmpsrc;
    for(int i = 0; i < 3; i++){
        tmpdst = {dst.x + (dst.w*2/7)*i, dst.y, dst.w*2/7, dst.h};
        nums[tmpnum[i]].Draw(&tmpdst);
    }
    tmpdst = {dst.x + dst.w/7 * 6.0, dst.y, dst.w/7, dst.h};
    str_s.Draw(&tmpdst);

}

void TimerUI::Set(TTF_Font *font){
    double timeMagW = 0.45, timeMagH = 0.56;
    timer.Set(font);
    base.LoadImg(basefile);
    rotate.LoadImg(rotatefile);
    dst = {Wd_Width*36/40 - UI_MAGNIFICATION * base.GetImgRect().w/2, Wd_Height * 5/30 - UI_MAGNIFICATION * base.GetImgRect().h/2, UI_MAGNIFICATION * base.GetImgRect().w, UI_MAGNIFICATION * base.GetImgRect().h};
    timeDst = {Wd_Width*36/40 - UI_MAGNIFICATION * timeMagW * base.GetImgRect().w/2, Wd_Height * 16/90 - UI_MAGNIFICATION * timeMagH * base.GetImgRect().h/2, UI_MAGNIFICATION * timeMagW * base.GetImgRect().w, UI_MAGNIFICATION * timeMagH * base.GetImgRect().h};
}

void TimerUI::Draw(float dir, float time){
    base.Draw(&dst);
    rotate.Draw(&dst, NULL, dir);
    timer.DrawTime(time, timeDst);
}

void PlayerNameUI::Set(const PlayerData *pData, TTF_Font *font){
    double nameMagW = 0.80, NameMagH = 0.65, idMagW = 0.18, idMagH = 0.48;
    int id = GetMyID();
    char idstr[3];
    sprintf(idstr,"%02d",id);
    SDL_Color color {255,255,255,255};
    base.LoadImg(basefile);
    name.LoadText(font,pData->name, color);
    ID.LoadText(font, idstr, color);
    baseDst = {Wd_Width * 35/40 - UI_MAGNIFICATION * base.GetImgRect().w/2, Wd_Height * 24/30 - UI_MAGNIFICATION * base.GetImgRect().h/2, UI_MAGNIFICATION * base.GetImgRect().w, UI_MAGNIFICATION * base.GetImgRect().h};
    nameDst = {Wd_Width * 71/80 - UI_MAGNIFICATION * nameMagW * base.GetImgRect().w/2, Wd_Height * 51/60 - UI_MAGNIFICATION * NameMagH * base.GetImgRect().h/2, UI_MAGNIFICATION * nameMagW * base.GetImgRect().w, UI_MAGNIFICATION * NameMagH * base.GetImgRect().h};
    idDst = {Wd_Width * 307/320 - UI_MAGNIFICATION * idMagW * base.GetImgRect().w/2, Wd_Height * 87/120 - UI_MAGNIFICATION * idMagH * base.GetImgRect().h/2, UI_MAGNIFICATION * idMagW * base.GetImgRect().w, UI_MAGNIFICATION * idMagH * base.GetImgRect().h};
}

void PlayerNameUI::Draw(){
    base.Draw(&baseDst);
    name.Draw(&nameDst);
    ID.Draw(&idDst);
}

void DirectionUI::Set(){
    base.LoadImg(basefile);
    rotate.LoadImg(rotatefile);
    Playerdir.LoadImg(dirfile);
    meterRect.LoadImg(meterfile);

    dst = {Wd_Width * 4/40 - UI_MAGNIFICATION * base.GetImgRect().w/2, Wd_Height * 5/30 - UI_MAGNIFICATION * base.GetImgRect().h/2, UI_MAGNIFICATION * base.GetImgRect().w, UI_MAGNIFICATION * base.GetImgRect().h};
}

void DirectionUI::Draw(float playerdir, float rotdir){

    FloatRect meterSrc = {0, 1-0.3*sin(5*rotdir)*sin(5*rotdir), 1, 1};
    FloatRect meterDst = {dst.x, dst.y + dst.h*(1-0.3*sin(5*rotdir)*sin(5*rotdir)), dst.w, dst.h};
    base.Draw(&dst);
    rotate.Draw(&dst,NULL,rotdir);
    Playerdir.Draw(&dst,NULL,playerdir);
    meterRect.Draw(&meterDst, &meterSrc);
}

void MapUI::Set(){
    base.LoadImg(basefile);
    dst = {Wd_Width * 4/40 - UI_MAGNIFICATION * base.GetImgRect().w/2, Wd_Height * 21/30 - UI_MAGNIFICATION * base.GetImgRect().h/2, UI_MAGNIFICATION * base.GetImgRect().w, UI_MAGNIFICATION * base.GetImgRect().h};
}

void MapUI::Draw(){
    base.Draw(&dst);
}

void Gui2D::Set(){
    font = TTF_OpenFont(fontpath,2000);
    if(font == NULL) fprintf(stderr,"ttf font cannot loaded\n");
    pData = GetPlayerData();
    dst = {0,0,Wd_Width,Wd_Height};
    back.LoadImg(basefile);
    timer.Set(font);
    playerName.Set(pData,font);
    direction.Set();
    minimap.Set();
    fprintf(stderr,"finish to load 2Dimg\n");
}

void Gui2D::Draw(){
    back.Draw(&dst);
    timer.Draw(rotdir,time);
    playerName.Draw();
    direction.Draw(-(pData->direction), rotdir);
    minimap.Draw();
    rotdir += 0.001;
    if(rotdir >= 2* PI)rotdir -= 2*PI;
}
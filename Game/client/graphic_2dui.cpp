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
    int id = GetSystem().GetMyID();
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

void MiniMap::SetMap(){
    const int (*terrainData)[MAP_SIZE_H][MAP_SIZE_D] = GetSystem().GetClientMap().GetTerrainData();
    int map_w = GetSystem().GetClientMap().GetMapW() , map_h = GetSystem().GetClientMap().GetMapH() , map_d = GetSystem().GetClientMap().GetMapD();
    Uint32 rmask, gmask, bmask, amask;
    int MiniMap_Mag = 10;

    /* SDLはピクセルを32bitの値として解釈する.
       よって, マスクはマシンのエンディアン(バイト順)に依存する */
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif

    SDL_Surface *img = SDL_CreateRGBSurface(0, map_w * MiniMap_Mag, map_d * MiniMap_Mag, 32, rmask, gmask, bmask, amask);
    SDL_Rect dst = {0, 0, MiniMap_Mag, MiniMap_Mag};
    for(int w = 0; w < map_w; w++){
        for(int d = 0; d < map_d; d++){
            int count = 0;
            for(int h = 0; h < map_h; h++){
                if (terrainData[w][h][d] == NomalBlock) count++;
                else if(terrainData[w][h][d] == GoalBlock){
                    count = -1;
                    break;
                }else break;
            }
            Uint32 color;
            dst.x = MiniMap_Mag * w;
            dst.y = MiniMap_Mag * d;
            if(count == 0) color = SDL_MapRGBA(img->format, 0, 0, 0, 0);
            else if(count > 0) color = SDL_MapRGB(img->format, 0, (int)((double)(count/map_h) * 255), 0);
            else color = SDL_MapRGB(img->format, 255, 255, 0);
            SDL_FillRect(img, &dst,color);
        }
    }
    imgRect.x = 0;
    imgRect.y = 0;
    imgRect.w = img->w;
    imgRect.h = img->h;
    src = {0,0,1,1};
    //テクスチャの生成
    glGenTextures(1,&Texture);
    glBindTexture(GL_TEXTURE_2D,Texture);
    int Mode = GL_RGB;
    if(img->format->BytesPerPixel == 4){
        Mode = GL_RGBA;
    }
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);

    glTexImage2D(GL_TEXTURE_2D, 0, img->format->BytesPerPixel, img->w, img->h, 0, Mode, GL_UNSIGNED_BYTE, img->pixels);
    SDL_FreeSurface(img);
}

void MiniMap::SetDstRect(int x, int y, int w, int h){
    if(w <= 0) dst.w = src.w;
    else dst.w = w;
    if(h <= 0) dst.h = src.h;
    else dst.h = h;
    dst.x = x;
    dst.y = y;
}

void MiniMap::SetSrcRect(float x, float y, float w, float h){
    if(w <= 0) src.w = imgRect.w;
    else src.w = imgRect.w;
    if(h <= 0) src.h = src.h;
    else src.h = h;
    if(x < 0) src.x = 0;
    else src.x = x;
    if(y < 0) src.y = 0;
    else src.y = y;
}

void MiniMap::Draw(FloatRect *argDst, FloatRect *argSrc, float dir){

    if(argDst != NULL) dst = *argDst;
    if(argSrc != NULL) src = *argSrc;

    Vector2 dstPoint[4], center = {dst.x + dst.w/2, dst.y + dst.h/2};
    dstPoint[0].x = center.x + cos(dir) * (dst.x - center.x) - sin(dir) * (dst.y - center.y);
    dstPoint[0].y = center.y + sin(dir) * (dst.x - center.x) + cos(dir) * (dst.y - center.y);
    dstPoint[1].x = center.x + cos(dir) * (dst.x + dst.w - center.x) - sin(dir) * (dst.y - center.y);
    dstPoint[1].y = center.y + sin(dir) * (dst.x + dst.w - center.x) + cos(dir) * (dst.y - center.y);
    dstPoint[2].x = center.x + cos(dir) * (dst.x + dst.w - center.x) - sin(dir) * (dst.y + dst.h - center.y);
    dstPoint[2].y = center.y + sin(dir) * (dst.x + dst.w - center.x) + cos(dir) * (dst.y + dst.h - center.y);
    dstPoint[3].x = center.x + cos(dir) * (dst.x - center.x) - sin(dir) * (dst.y + dst.h - center.y);
    dstPoint[3].y = center.y + sin(dir) * (dst.x - center.x) + cos(dir) * (dst.y + dst.h - center.y);


    
    glBindTexture(GL_TEXTURE_2D,Texture);
    glBegin(GL_QUADS);
        glTexCoord2f(src.x, src.y); glVertex3f(dstPoint[0].x, dstPoint[0].y, 0);
        glTexCoord2f(src.x + src.w, src.y); glVertex3f(dstPoint[1].x, dstPoint[1].y, 0);
        glTexCoord2f(src.x + src.w, src.y + src.h); glVertex3f(dstPoint[2].x, dstPoint[2].y, 0);
        glTexCoord2f(src.x, src.y + src.h); glVertex3f(dstPoint[3].x, dstPoint[3].y, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D,0);
}

void MapUI::Set(){
    base.LoadImg(basefile);
    minimap.SetMap();
    dst = {Wd_Width * 4/40 - UI_MAGNIFICATION * base.GetImgRect().w/2, Wd_Height * 21/30 - UI_MAGNIFICATION * base.GetImgRect().h/2, UI_MAGNIFICATION * base.GetImgRect().w, UI_MAGNIFICATION * base.GetImgRect().h};
}

void MapUI::Draw(){
    base.Draw(&dst);
    minimap.Draw(&dst,NULL,PI);
}

void Gui2D::Set(){
    font = TTF_OpenFont(fontpath,2000);
    if(font == NULL) fprintf(stderr,"ttf font cannot loaded\n");
    pData = GetSystem().GetPlayerData() + GetSystem().GetMyID();
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
    direction.Draw(-(pData->direction.horizontal), rotdir);
    minimap.Draw();
    rotdir += 0.01;
    if(rotdir >= 2* PI)rotdir -= 2*PI;
}

#include "image.hpp"

const double PI = 3.141592;

void Image::LoadImg(const char *filename){
    SDL_Surface *img = NULL;
    img = IMG_Load(filename);
    if(!img){
        //エラー処理
        fprintf(stderr,"image file \"%s\" cannot loaded\n",filename);
    }else{
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
        //fprintf(stderr,"TextureNo.%d:finish to load imgfile:%s\n",Texture,filename);
    }
}

void Image::LoadText(TTF_Font *font, const char *text, SDL_Color color){
    SDL_Surface *img = NULL;
    img = TTF_RenderText_Blended(font, text, color);
    if(!img){
        fprintf(stderr,"text image \"%s\" cannot loaded\n",text);
    }else{
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
        //fprintf(stderr,"TextureNo.%d:finish to load txtimg:%s\n",Texture,text);
    }
}

void Image::SetDstRect(int x, int y, int w, int h){
    if(w <= 0) dst.w = src.w;
    else dst.w = w;
    if(h <= 0) dst.h = src.h;
    else dst.h = h;
    dst.x = x;
    dst.y = y;
}

void Image::SetSrcRect(float x, float y, float w, float h){
    if(w <= 0) src.w = imgRect.w;
    else src.w = imgRect.w;
    if(h <= 0) src.h = src.h;
    else src.h = h;
    if(x < 0) src.x = 0;
    else src.x = x;
    if(y < 0) src.y = 0;
    else src.y = y;
}

void Image::Draw(FloatRect *argDst, FloatRect *argSrc, float dir){

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
    // dstPoint[0].x = dst.x;
    // dstPoint[0].y = dst.y;
    // dstPoint[1].x = dst.x+dst.w;
    // dstPoint[1].y = dst.y+dst.h;

    
    glBindTexture(GL_TEXTURE_2D,Texture);
    glBegin(GL_QUADS);
        glTexCoord2f(src.x, src.y); glVertex3f(dstPoint[0].x, dstPoint[0].y, 0);
        glTexCoord2f(src.x + src.w, src.y); glVertex3f(dstPoint[1].x, dstPoint[1].y, 0);
        glTexCoord2f(src.x + src.w, src.y + src.h); glVertex3f(dstPoint[2].x, dstPoint[2].y, 0);
        glTexCoord2f(src.x, src.y + src.h); glVertex3f(dstPoint[3].x, dstPoint[3].y, 0);
    glEnd();
    glBindTexture(GL_TEXTURE_2D,0);
    
}
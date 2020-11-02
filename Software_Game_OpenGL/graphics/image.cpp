#include "image.hpp"

void Image::LoadImg(const char *filename){
    SDL_Surface *img = NULL;
    img = IMG_Load(filename);
    if(!img){
        //エラー処理
        printf("image file \"%s\" cannot loaded",filename);
    }else{
        w = img->w;
        h = img->h;
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

        glTexImage2D(GL_TEXTURE_2D, 0, img->format->BytesPerPixel, w, h, 0, Mode, GL_UNSIGNED_BYTE, img->pixels);
		SDL_FreeSurface(img);
    }
}

void Image::LoadText(TTF_Font *font, const char *text, SDL_Color color){
    SDL_Surface *img = NULL;
    img = TTF_RenderText_Blended(font, text, color);
    if(!img){
        printf("ttf font cannot loaded");
    }else{
        w = img->w;
        h = img->h;
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

        glTexImage2D(GL_TEXTURE_2D, 0, img->format->BytesPerPixel, w, h, 0, Mode, GL_UNSIGNED_BYTE, img->pixels);
		SDL_FreeSurface(img);
    }
}

void Image::Draw(int x,int y){

    glBindTexture(GL_TEXTURE_2D,Texture);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0); glVertex3f(x, y, 0);
        glTexCoord2f(1, 0); glVertex3f(x + w, y, 0);
        glTexCoord2f(1, 1); glVertex3f(x + w, y + h, 0);
        glTexCoord2f(0, 1); glVertex3f(x, y + h, 0);
    glEnd();
    
}
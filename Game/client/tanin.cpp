#include"init_name.hpp"

//
    SDL_Rect text_init_rect = {244, 296, 304, 32};
    SDL_Surface *surface;
    SDL_Texture *texture;
    TTF_Font *font;

    
//

void init_charactor_name(SDL_Renderer *renderer ,char name[32] , TTF_Font *T_font){
    char text[32] = {};
    char texted[32] = {};
    char composition[32] = {};
    int flug = 1;
    SDL_Event event;
    int iw,ih;

    int count = 0;
    int sflug = 0;
    
    font = T_font;
    SDL_StartTextInput();
    while(flug){
        SDL_RenderClear(renderer);
        if(SDL_PollEvent(&event)){
            switch(event.type){
                case SDL_TEXTINPUT:
                if(strlen(name) >=32) break;
                    sflug = 0;
                    /* テキストの末尾に新しいテキストを追加する */
                    sprintf(text,"%s%s",text,event.text.text);
                    sprintf(name,text);
                    drow_TTF(renderer , name , &iw , &ih);
                    //printf("TEXTINIT_%s\n",text);
                break;
                case SDL_TEXTEDITING:
                if(strlen(name) >=32) break;
                    sflug = 1;
                    SDL_SetTextInputRect(&text_init_rect);
                    sprintf(composition,"%s%s",text,event.edit.text);
                    sprintf(name,composition);
                    drow_TTF(renderer , name , &iw , &ih);
                    //printf("TEXTEDITING_%d_%d_%s\n",(int)cursor,(int)selection_len,composition);
                    //drowbox(renderer , {320 , 296 , iw , ih},0,0,255,128);
                break;
                case SDL_KEYDOWN:
                    if(event.key.keysym.sym == SDLK_BACKSPACE){
                        count = strlen(text);
                        //printf("%d_%s\n",count,text);
                        if(count>0){
                            strncpy(texted , text , count - 1);
                            sprintf(text,texted);
                            memset(texted,'\0',32);
                            drow_TTF(renderer , text , &iw , &ih);
                        }
                    }
                    if(event.key.keysym.sym == SDLK_ESCAPE || (sflug == 0 && event.key.keysym.sym == SDLK_RETURN)) {
                        flug = 0;
                    }
                break;
            }
        }
    }
    SDL_StopTextInput();
}

void drow_TTF(SDL_Renderer *renderer , char* text ,int *iw,int *ih){
    surface = TTF_RenderUTF8_Blended(font , text ,{255,255,255,255});
    texture = SDL_CreateTextureFromSurface(renderer , surface);
    SDL_QueryTexture(texture, NULL, NULL, iw, ih);

    SDL_Rect txtRect={0,0,*iw,*ih};
    SDL_Rect pasteRect={320-(*iw/2),296,*iw,*ih};
    SDL_RenderCopy(renderer,texture,&txtRect,&pasteRect);
    SDL_RenderPresent(renderer); 

}

#include "client_KeybordInput.h"


KeybordInput::KeybordInput() : _key(SDL_GetKeyboardState(NULL))
{}

KeybordInput::~KeybordInput() 
{}

void KeybordInput::GetInput(SDL_Event event){
    if(SDL_PollEvent(&event)){
        _Input = (InputType){false, false, false, false};
        if(_key[SDL_SCANCODE_ESCAPE]){
            _Input.end = true;
        }
        /************/
        /**移動関連**/ 
        /************/
        if(_key[SDL_SCANCODE_W]){
            _Input.Forward = true;
        }
        
        if(_key[SDL_SCANCODE_A]){
            _Input.Left = true;
        }
        else if(_key[SDL_SCANCODE_D]){
            _Input.Right = true;
        }

        if(_key[SDL_SCANCODE_SPACE]){
            _Input.Jump = true;
        }
        
        /************/
        /**配置関連**/
        /************/
        if(_key[SDL_SCANCODE_P]){
            _Input.Put = true;
        }
        
        if(_key[SDL_SCANCODE_UP]){
            _Input.U = true;
        }
        else if(_key[SDL_SCANCODE_DOWN]){
            _Input.D = true;
        }
        else if(_key[SDL_SCANCODE_RIGHT]){
            _Input.R = true;
        }
        else if(_key[SDL_SCANCODE_LEFT]){
            _Input.L = true;
        }
    }    
}

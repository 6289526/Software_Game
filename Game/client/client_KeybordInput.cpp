/**
 * @file client_KeybordInput.cpp
 * @brief キーボード入力管理
 * @version 0.1
 * @date 2021-01-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "client_KeybordInput.h"

KeybordInput::KeybordInput() : _key(SDL_GetKeyboardState(NULL))
{
}

KeybordInput::~KeybordInput()
{
}

void KeybordInput::UpdateInput()
{
	if (SDL_PollEvent(&_Event))
	{
		InputType t = _Input;
		_Input = t;
		
		if (_key[SDL_SCANCODE_ESCAPE])
		{
			_Input.End = true;
		}

		/************/
		/**移動関連**/
		/************/
		if (_key[SDL_SCANCODE_W])
		{
			_Input.Forward = true;
		}
		else
		{
			_Input.Forward = false;
		}

		if (_key[SDL_SCANCODE_A])
		{
			_Input.Right = false;
			_Input.Left = true;
		}
		else if (_key[SDL_SCANCODE_D])
		{
			_Input.Left = false;
			_Input.Right = true;
		}
		else
		{
			_Input.Right = false;
			_Input.Left = false;
		}

		if (_key[SDL_SCANCODE_SPACE])
		{
			_Input.Jump = true;
		}
		else
		{
			_jumpFlag = false;
		}

		/************/
		/**配置関連**/
		/************/
		if (_key[SDL_SCANCODE_P])
		{
			_Input.Put = true;
		}
		else
		{
			_putFlag = false;
		}

		if (_key[SDL_SCANCODE_UP])
		{
			_Input.U = true;
		}
		else
		{
			_Input.U = false;
		}
		if (_key[SDL_SCANCODE_DOWN])
		{
			_Input.D = true;
		}
		else
		{
			_Input.D = false;
		}

		if (_key[SDL_SCANCODE_RIGHT])
		{
			_Input.R = true;
		}
		else
		{
			_Input.R = false;
		}
		if (_key[SDL_SCANCODE_LEFT])
		{
			_Input.L = true;
		}
		else
		{
			_Input.L = false;
		}
	}
}

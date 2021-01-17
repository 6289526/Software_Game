/**
 * @file client_smart.cpp
 * @brief スマートフォン入力管理
 * @version 0.1
 * @date 2021-01-17
 * 
 * @copyright Copyright (c) 2021
 * 
 */
#include "client_smart.h"
/**
 * @brief Construct a new Smart:: Smart object
 * 
 */
Smart::Smart()
{
}

Smart::~Smart()
{
}

/**
 * @brief 入力を受け取る
 * 
 */
void Smart::UpdateInput()
{

	InputType t = _Input;
	_Input = t;

	if (_setname)
	{
		if (GoSInput.L1)
		{
			_Input.Forward = true;
		}
		else
		{
			_Input.Forward = false;
		}

		if (GoSInput.R1)
		{
			_Input.Jump = true;
			
		}
		else
		{
			_Input.Jump = false;
		}

		if (GoSInput.R2)
		{
			_Input.Jump = true;
			
		}
		else
		{
			_Input.Jump = false;
		}
		
		if (GoSInput.R3)
		{
			_Input.Up = true;
		}
		else
		{
			_Input.Up = false;
		}

		if (GoSInput.R4)
		{
			_Input.Left = true;
		}
		else
		{
			_Input.Left = false;
		}
		if (GoSInput.R5)
		{
			_Input.Right = true;
		}
		else
		{
			_Input.Right = false;
		}
		
		if (GoSInput.R6)
		{
			_Input.Down = true;
		}
		else
		{
			_Input.Down = false;
		}
	}
	else
	{
		if (GoSInput.Q)
		{
			_Input.End = true;
		}
		/************/
		/**移動関連**/
		/************/
		if (GoSInput.L1)
		{
			_Input.Forward = true;
		}
		else
		{
			_Input.Forward = false;
		}

		if (GoSInput.R4)
		{
			_Input.Right = false;
			_Input.Left = true;
		}
		else if (GoSInput.R5)
		{
			_Input.Left = false;
			_Input.Right = true;
		}
		else
		{
			_Input.Right = false;
			_Input.Left = false;
		}

		if (GoSInput.R2)
		{
			_Input.Jump = true;
		}
		else
		{
			_jumpFlag = false;
			_Input.Jump = false;
		}

		if(GoSInput.R3){
			_Input.U = true;
		}
		else{
			_Input.U = false;
		}
		if(GoSInput.R6){
			_Input.D = true;
		}
		else{
			_Input.D = false;
		}

		/************/
		/**配置関連**/
		/************/
		if (GoSInput.R1)
		{
			_Input.Put = true;
		}
		else
		{
			_putFlag = false;
			_Input.Put = false;
		}
	}
}

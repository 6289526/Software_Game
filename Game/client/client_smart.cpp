#include "client_smart.h"

Smart::Smart()
{
}

Smart::~Smart()
{
}

void Smart::UpdateInput()
{

	InputType t = _Input;
	_Input = t;

	if (GoSInput.E)
	{
		_Input.End = true;
	}

	/************/
	/**移動関連**/
	/************/
	if (GoSInput.F)
	{
		_Input.Forward = true;
	}
	else
	{
		_Input.Forward = false;
	}

	if (GoSInput.L)
	{
		_Input.Right = false;
		_Input.Left = true;
	}
	else if (GoSInput.R)
	{
		_Input.Left = false;
		_Input.Right = true;
	}
	else
	{
		_Input.Right = false;
		_Input.Left = false;
	}

	if (GoSInput.U)
	{
		_Input.Jump = true;
	}
	else
	{
		_jumpFlag = false;
		_Input.Jump = false;
	}

	/************/
	/**配置関連**/
	/************/
	if (GoSInput.P)
	{
		_Input.Put = true;
	}
	else
	{
		_putFlag = false;
		_Input.Put = false;
	}

	if (GoSInput.Up)
	{
		_Input.Up = true;
	}
	else
	{
		_Input.Up = false;
	}

	if (GoSInput.Down)
	{
		_Input.Down = true;
	}
	else
	{
		_Input.Down = false;
	}
}

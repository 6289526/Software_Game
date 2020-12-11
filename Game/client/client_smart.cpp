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

	if (_setname)
	{
		if (GoSInput.L1)
		{
			_Input.Up = true;
		}
		else
		{
			_Input.Up = false;
		}

		if (GoSInput.L2)
		{
			_Input.Down = true;
		}
		else
		{
			_Input.Down = false;
		}

		if (GoSInput.R1)
		{
			_Input.Left = true;
			_Input.Right = false;
		}
		else if (GoSInput.R2)
		{
			_Input.Left = false;
			_Input.Right = true;
		}
		else
		{
			_Input.Left = false;
			_Input.Right = false;
		}

		if (GoSInput.R3)
		{
			_Input.Jump = true;
		}
		else
		{
			_Input.Jump = false;
		}

		if (GoSInput.R4)
		{
			_Input.Forward = true;
		}
		else
		{
			_Input.Forward = false;
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
		if (GoSInput.L1 || GoSInput.L2)
		{
			_Input.Forward = true;
		}
		else
		{
			_Input.Forward = false;
		}

		if (GoSInput.R1)
		{
			_Input.Right = false;
			_Input.Left = true;
		}
		else if (GoSInput.R2)
		{
			_Input.Left = false;
			_Input.Right = true;
		}
		else
		{
			_Input.Right = false;
			_Input.Left = false;
		}

		if (GoSInput.R4)
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
		if (GoSInput.R3)
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

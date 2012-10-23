/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman & Petr Tomicek 2012.
*/

#include "KeyState.h"
#include <assert.h>

bool UpdateKeyState(bool keyPress,KeyState& state)
{
	bool stateChange = true;

	if(keyPress)
	{
		switch(state)
		{
		case KeyState_NotPressed:
			{
				state = KeyState_Pressed;
			}
			break;

		case KeyState_Pressed:
			{
				state = KeyState_Held;
			}
			break;

		case KeyState_Held:
			{
				state = KeyState_Held;
			}
			break;

		case KeyState_Released:
			{
				state = KeyState_Pressed;
			}
			break;

		default:
			assert(0);
			break;
		}
	}
	else
	{
		switch(state)
		{
		case KeyState_NotPressed:
			{
				state = KeyState_NotPressed;
				stateChange = false;
			}
			break;

		case KeyState_Pressed:
			{
				state = KeyState_Released;
			}
			break;

		case KeyState_Held:
			{
				state = KeyState_Released;
			}
			break;

		case KeyState_Released:
			{
				state = KeyState_NotPressed;
			}
			break;

		default:
			assert(0);
			break;
		}
	}

	return stateChange;
}
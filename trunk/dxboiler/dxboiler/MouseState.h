/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman & Petr Tomicek 2012.
*/

#ifndef MouseState_h__
#define MouseState_h__

#include <xnamath.h>
#include "KeyState.h"

struct MouseState
{
	MouseState():WheelMovement(0)
	{
		LeftButton = KeyState_NotPressed;
		RightButton = KeyState_NotPressed;
		MiddleButton = KeyState_NotPressed;
	}

  KeyState LeftButton;
	KeyState RightButton;
	KeyState MiddleButton;
	float WheelMovement;
	XMFLOAT2 Position;
	XMFLOAT2 RelativeMovement;
	XMFLOAT2 ScreenSpacePosition;
};

#endif // MouseState_h__

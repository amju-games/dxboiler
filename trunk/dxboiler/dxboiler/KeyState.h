/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman & Petr Tomicek 2012.
*/

#ifndef KeyState_h__
#define KeyState_h__

enum KeyState
{
	KeyState_NotPressed=0,
	KeyState_Pressed,
	KeyState_Held,
	KeyState_Released,
	KeyState_MaxKeyStates
};


bool UpdateKeyState(bool keyPress,KeyState& state);

#endif // KeyState_h__
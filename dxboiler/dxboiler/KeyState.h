#ifndef KeyState_h__
#define KeyState_h__

typedef enum KeyState
{
	KeyState_NotPressed=0,
	KeyState_Pressed,
	KeyState_Held,
	KeyState_Released,
	KeyState_MaxKeyStates
}KeyState;


bool UpdateKeyState(bool keyPress,KeyState& state);

#endif // KeyState_h__
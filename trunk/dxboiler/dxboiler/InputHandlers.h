#ifndef InputHandlers_h__
#define InputHandlers_h__

#include "MouseState.h"

class MouseInputHandler
{
public:
	virtual bool ProcessMouseEvent(MouseState& state) = 0;
};

class KeyInputHandler
{
public:
	virtual bool ProcessKeyboardEvent(Key& key) = 0; 
};
#endif // InputHandlers_h__


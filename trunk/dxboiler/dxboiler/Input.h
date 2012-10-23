/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman & Petr Tomicek 2012.
*/

#ifndef INPUT_H
#define INPUT_H


#include <vector>
#include <Windows.h>
#include "Key.h"
#include "InputHandlers.h"
#include <map>

class Input
{
public:
	Input();
	bool Init(HWND* window);
	void Update();
	void AddInputMessage(RAWINPUT*);
	void AddMouseHandler(int,MouseInputHandler*);
	void AddKeyBoardHandler(int,KeyInputHandler*);
	~Input();
private:

	class Keyboard 
	{
	public:
		Keyboard();
		bool Init();
		void ProcessKey(RAWKEYBOARD& key);
		void Addhandler(int,KeyInputHandler*);
		void Update();
		~Keyboard();
	private:
		std::map<short,Key> m_keys;
		std::multimap<int,KeyInputHandler*> m_handlersMap;
	};

	class Mouse 
	{
	public:
		Mouse();
		bool Init(HWND* window);
		void ProcessMouse(RAWMOUSE& mouse);
		void AddHandler(int,MouseInputHandler*);
		~Mouse();
		void UpdateHandlers();
	private:
		MouseState m_state;
		HWND* m_window;
		USHORT m_lastButtonState;
		bool m_leftDown;
		bool m_rightDown;
		bool m_middleDown;
		bool m_update;
		std::multimap<int,MouseInputHandler*> m_handlersMap;
	
	};

	std::vector< RAWINPUT*> m_rawInputHandles;
	Keyboard m_keyBoard;
	Mouse m_mouse;
};

#endif
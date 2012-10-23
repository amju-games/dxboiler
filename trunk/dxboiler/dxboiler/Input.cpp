/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman & Petr Tomicek 2012.
*/

#include <iostream>
#include "Input.h"
#include "ReportError.h"
#include "Key.h"

bool Input::Init( HWND* window )
{
	if (!m_keyBoard.Init())
	{
		return false;
	}
	
	if (!m_mouse.Init(window))
	{
		return false;
	}

	return true;
}

void Input::Update()
{
	for (unsigned int i = 0; i < m_rawInputHandles.size(); ++i)
	{
		RAWINPUT* raw = m_rawInputHandles[i];

		if (!raw)
		{
			ReportError("RawInput Invalid!!!");
		}
		switch(raw->header.dwType)
		{
		case RIM_TYPEKEYBOARD:
			m_keyBoard.ProcessKey(raw->data.keyboard);
			break;
		case RIM_TYPEMOUSE:
			m_mouse.ProcessMouse(raw->data.mouse);
			break;
		default:
			break;
		}

		delete m_rawInputHandles[i];
	}
	
	m_rawInputHandles.clear();
	m_mouse.UpdateHandlers();
}

void Input::AddInputMessage( RAWINPUT* raw)
{
	m_rawInputHandles.push_back(raw);
}

Input::Input()
{
}

Input::~Input()
{
}

void Input::AddMouseHandler( int prio,MouseInputHandler* pMH)
{
	m_mouse.AddHandler(prio,pMH);
}

void Input::AddKeyBoardHandler( int,KeyInputHandler* )
{
}

Input::Keyboard::Keyboard()
{
}

Input::Keyboard::~Keyboard()
{
}

bool Input::Keyboard::Init()
{
	RAWINPUTDEVICE Raw;

	//Add keyboard
	Raw.hwndTarget = 0;
	Raw.usUsagePage = 0x01;
	Raw.usUsage = 0x06;
	Raw.dwFlags = 0;

	if (RegisterRawInputDevices(&Raw,1,sizeof(Raw)))
	{
		Key tmp;
		int c = 0;
		for (int i = KEY_0; i < (KEY_0 + 10); ++i)
		{
			tmp.state = KeyState_NotPressed;
			tmp.ascii = char(48+c);
			tmp.code = (KeyCode)i;
			m_keys[i] = tmp;
			++c;
		}

		c = 0;
		for (int i = KEY_A; i < (KEY_A + 26); ++i)
		{
			tmp.state = KeyState_NotPressed;
			tmp.ascii = char(97+c);
			tmp.code = (KeyCode)i;
			m_keys[i] = tmp;
			++c;
		}

		c = 0;
		for (int i = KEY_NUM0; i < KEY_NUM0 + 10; ++i)
		{
			tmp.state = KeyState_NotPressed;
			tmp.ascii = char(48+c);
			tmp.code = (KeyCode)i;
			m_keys[i] = tmp;
			++c;
		}
	}
	return true;
}

void Input::Keyboard::ProcessKey( RAWKEYBOARD& key )
{
	auto it  = m_keys.find(key.VKey);

	if (it != m_keys.end())
	{
		Key& currentKey = it->second;

		switch (key.Flags)
		{
		case 0: // Pressed
			UpdateKeyState(true,currentKey.state);
			break;
		case 1: // Released
			UpdateKeyState(false,currentKey.state);
			break;
		}

		if (currentKey.state == KeyState_Pressed)
		{
			std::cout << "Key " << currentKey.ascii << " is pressed \n";
		}
		if (currentKey.state == KeyState_Held)
		{
			std::cout << "Key " << currentKey.ascii << " is held \n";
		}
		if (currentKey.state == KeyState_Released)
		{
			std::cout << "Key " << currentKey.ascii << " is released \n";
		}
	}
}

void Input::Keyboard::Update()
{
}

void Input::Keyboard::Addhandler( int prio,KeyInputHandler* handler)
{
	m_handlersMap.insert(std::pair<int, KeyInputHandler*>(prio,handler));
}


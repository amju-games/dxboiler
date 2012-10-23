/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman & Petr Tomicek 2012.
*/

#include <iostream>
#include "Input.h"

Input::Mouse::Mouse():m_lastButtonState(0),m_window(NULL),m_leftDown(false),m_rightDown(false),m_middleDown(false)
{
}

bool Input::Mouse::Init(HWND* window)
{
	RAWINPUTDEVICE Raw;

	Raw.hwndTarget = 0;
	Raw.usUsagePage = 0x01;
	Raw.usUsage = 0x02;
	Raw.dwFlags = 0;

	m_window = window;

	return (RegisterRawInputDevices(&Raw,1,sizeof(Raw)) == TRUE);
}

void Input::Mouse::UpdateHandlers()
{
	for (auto it = m_handlersMap.begin(); it != m_handlersMap.end(); ++it)
	{
		if (it->second->ProcessMouseEvent(m_state))
		{
			break;
		}
	}

	m_update = false;
}

void Input::Mouse::AddHandler( int prio,MouseInputHandler* pMH)
{
	m_handlersMap.insert(std::pair<int, MouseInputHandler*>(prio,pMH));
}

Input::Mouse::~Mouse()
{
}

void Input::Mouse::ProcessMouse( RAWMOUSE& mouse )
{
	m_update = true;
	if (mouse.lLastX && mouse.lLastY)
	{
		POINT mouseCoord;
		GetCursorPos(&mouseCoord);
		ScreenToClient((*m_window),&mouseCoord);
		RECT client;
		GetClientRect((*m_window),&client);

		if(mouseCoord.x < 0)
		{
			mouseCoord.x = 0;
		}

		if(mouseCoord.x > client.right)
		{
			mouseCoord.x = client.right;
		}

		if(mouseCoord.y < 0)
		{
			mouseCoord.y = 0;
		}

		if(mouseCoord.y > client.bottom)
		{
			mouseCoord.y = client.bottom;
		}

		m_state.Position.x = (float)mouseCoord.x;
		m_state.Position.y = (float)mouseCoord.y;
		m_state.RelativeMovement.x = (float)mouse.lLastX;
		m_state.RelativeMovement.y = (float)mouse.lLastY;

		if (mouseCoord.x != 0)
		{
			m_state.ScreenSpacePosition.x = (float)mouseCoord.x / (float)client.right;
		}
		else m_state.ScreenSpacePosition.x = 0;
		
		if (mouseCoord.y != 0)
		{
			m_state.ScreenSpacePosition.y = (float)mouseCoord.y / (float)client.bottom ;
		}
		else m_state.ScreenSpacePosition.y = 0;
	}
  
	// checking left button 
	if ((mouse.usButtonFlags & 0x0001))
	{
		m_leftDown = true;
	}

	if ((mouse.usButtonFlags & 0x0002))
	{
		m_leftDown = false;
	}

	//checking right button 
	if ((mouse.usButtonFlags & 0x0004))
	{
		m_rightDown = true;
	}

	if ((mouse.usButtonFlags & 0x0008))
	{
		m_rightDown = false;
	}

	// checking middle button 
	//if ((mouse.usButtonFlags & 0x0010) != (m_lastButtonState & 0x0010))
	//{
	//	float test = 0;
	//}

	if ((mouse.usButtonFlags & 0x0400))
	{
		m_state.WheelMovement = mouse.usButtonData;
	}
	else
	{
		m_state.WheelMovement = 0;
	}
	//std::cout << "Mouse: " << "X: " << m_state.Position.x << " Y: " << m_state.Position.y << "\n\n";

	UpdateHandlers();
}
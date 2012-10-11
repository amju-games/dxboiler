#include "FullScreenShader.h"
#include <iostream>

void FullScreenShader::UpdateConstantBuffer()
{
	m_cb.mouseCoord = m_mouseCoord;
}

bool FullScreenShader::ProcessMouseEvent( MouseState& ms)
{
	m_mouseCoord = ms.ScreenSpacePosition;
//	std::cout << "U:" << m_mouseCoord.x << " V:"  << m_mouseCoord.y << "\n";
	return true;
}

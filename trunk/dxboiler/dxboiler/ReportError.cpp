/*
DX Boiler: boilerplate code for DX11 projects.
This source code (c) Copyright Jason Colman 2012.
*/

#include "ReportError.h"

#ifdef WIN32
#include <Windows.h>
#endif

void ReportError(const std::string& errorMsg)
{
#ifdef WIN32
  MessageBoxA(0, errorMsg.c_str(), "OH DEAR GOD!!!!", MB_ICONSTOP);
#endif
}


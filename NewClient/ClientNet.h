#pragma once
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <windows.h>

extern SOCKET m_SOCK;
extern short m_PortNum;




BOOL connentNetWork();
BOOL NetworkProc(WPARAM wParam, LPARAM lParam);
BOOL networkClean();

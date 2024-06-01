#pragma once
#include"pch.h"

struct WindowStyle
{
	DWORD		m_dwWindowExStyle = WS_EX_APPWINDOW;
	DWORD		m_dwWindowStyle = WS_OVERLAPPEDWINDOW;
	DWORD		m_dwWindowPosX;
	DWORD		m_dwWindowPosY;
	DWORD		m_dwWindowWidth;
	DWORD		m_dwWindowHeight;

};

class MyWindow
{
private:
	HINSTANCE	m_hInstance; //컴퓨터 저장공간에서 할당된 실체를 의미한다.
	HWND		m_hWnd;
	WindowStyle m_WindowStyle;

public:
	HINSTANCE	GetHInstance() const { return m_hInstance; } //컴퓨터 저장공간에서 할당된 실체를 의미한다.
	HWND		GetHWND() const { return m_hWnd; }
	WindowStyle GetWindowStyle() const { return m_WindowStyle; }

public:

	bool SetRegisterClassWindow(HINSTANCE hInstance);
	bool SetWindow(const WCHAR* sztitle, DWORD dwWindowwidth = 1920, DWORD dwWindowHeight = 1080);

public:
	MyWindow();
	virtual ~MyWindow();

};


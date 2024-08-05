#include "MyWindow.h"
#include "ClientNet.h"
#include"Input.h"
WindowStyle MyWindow::m_WindowStyle;
HWND MyWindow::m_hWnd;
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case dfNETWORK_UM:
    {
        if (!NetworkProc(wParam, lParam))
        {
            MessageBox(hWnd, L"Disconnect", L"Disconnect", MB_OK);
            PostQuitMessage(0);
        }
        break;
    }
    case WM_ACTIVATEAPP:
        Input::GetInstance().SetActive((bool)wParam);
        break;
    case WM_COMMAND:
    {
    
    }
    case WM_DESTROY:

        networkClean();
        MessageBox(hWnd, L"Disconnect", L"Disconnect", MB_OK);
        PostQuitMessage(0);

        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}


bool MyWindow::SetRegisterClassWindow(HINSTANCE hInstance)
{
	//������ ���� ����
	WNDCLASSEXW wcex;
	ZeroMemory(&wcex, sizeof(wcex));
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc; // ���������ν��� �����̺�Ʈ�� �޼��� ť�� ����
	wcex.hInstance = hInstance;
	wcex.lpszClassName = L"������";
	wcex.hCursor = LoadCursor(nullptr, IDC_CROSS);
	wcex.hbrBackground = (HBRUSH)CreateSolidBrush(RGB(190, 190, 70));
	WORD ret = RegisterClassExW(&wcex);

	m_hInstance = hInstance;

	return true;
}

bool MyWindow::SetWindow(const WCHAR* sztitle, DWORD dwWindowwidth, DWORD dwWindowHeight)
{
    m_WindowStyle.m_dwWindowWidth = dwWindowwidth;
    m_WindowStyle.m_dwWindowHeight = dwWindowHeight;

#ifndef _DEBUG
    m_dwWindowExStyle = WS_EX_TOPMOST;
    m_dwWindowStyle = WS_POPUPWINDOW;
#endif 

    RECT rc = { 0, 0,  static_cast<LONG>(m_WindowStyle.m_dwWindowWidth), static_cast<LONG>(m_WindowStyle.m_dwWindowHeight) };
    AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, FALSE);

    m_hWnd = CreateWindowEx(m_WindowStyle.m_dwWindowExStyle, L"������", sztitle,
        m_WindowStyle.m_dwWindowStyle,//������ �Ӽ�����
        CW_USEDEFAULT, CW_USEDEFAULT,//������ ��������
        rc.right - rc.left, rc.bottom - rc.top,//������ ũ��
        nullptr, nullptr, m_hInstance, nullptr);

    if (!m_hWnd)
    {
        return FALSE;
    }
    ShowWindow(m_hWnd, SW_SHOWNORMAL);

    return true;
}

MyWindow::MyWindow()
	:m_hInstance(0)
{





}

MyWindow::~MyWindow()
{
}





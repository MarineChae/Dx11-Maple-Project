#include "Input.h"
#include"Device.h"
bool Input::Init()
{
    ZeroMemory(&m_dwKeyState, sizeof(m_dwKeyState));
    return true;
}

bool Input::Frame()
{
    ::GetCursorPos(&m_MousePos);
    ::ScreenToClient(MyWindow::GetHWND(), &m_MousePos);

    for (int iKey = 0; iKey < 256; ++iKey)
    {
        //현재 키의 상태값을 가져옴
        SHORT sh = GetAsyncKeyState(iKey);
        //키가 눌려있는상태
        if (sh & 0x8000)
        {
            if (m_dwKeyState[iKey] == KEY_STATE::KEY_NONE)
            {
                m_dwKeyState[iKey] = KEY_STATE::KEY_PUSH;
            }
            else
            {
                m_dwKeyState[iKey] = KEY_STATE::KEY_HOLD;
            }
        }
        else
        {
            if (m_dwKeyState[iKey] == KEY_PUSH || m_dwKeyState[iKey] == KEY_HOLD)
            {
                m_dwKeyState[iKey] = KEY_UP;
            }
            else
            {
                m_dwKeyState[iKey] = KEY_NONE;
            }
        }
    }
    m_BeforeMousePos = m_MousePos;
    return true;
}

bool Input::Render()
{
#ifdef _DEBUG
   //std::wstring mousePos = std::to_wstring(m_MousePos.x);
   //mousePos += L",";
   //mousePos += std::to_wstring(m_MousePos.y);
   //mousePos += L"\n";
   //OutputDebugString(mousePos.c_str());
#endif
    return true;
}

bool Input::Release()
{
    return false;
}

Input::~Input()
{
}

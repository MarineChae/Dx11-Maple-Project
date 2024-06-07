#include "ClientNet.h"
#include"MyWindow.h"
SOCKET m_SOCK;
short m_PortNum = 12345;

BOOL connentNetWork()
{
    WSADATA wsa;
    if(0 != WSAStartup(MAKEWORD(2, 2), &wsa))
    {
        return false;
    }

    m_SOCK = socket(AF_INET, SOCK_STREAM, 0);
    if (INVALID_SOCKET == m_SOCK)
    {
        return false;
    }


    WSAAsyncSelect(m_SOCK, MyWindow::GetHWND(), dfNETWORK_UM, FD_READ | FD_WRITE | FD_CLOSE | FD_CONNECT);

    SOCKADDR_IN sa;

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr("127.0.0.1");
    sa.sin_port = htons(m_PortNum);

    int ret = connect(m_SOCK, (SOCKADDR*)&sa, sizeof(sa));
    if (ret == 0)
    {
        OutputDebugString(L"Connection Complete");

    }
    else
    {
        int err = WSAGetLastError();
        if (err == WSAEWOULDBLOCK)
        {
            OutputDebugString(L"WSAEWOULDBLOCK");
        }
        else
        {
            OutputDebugString(L"Connection Faild");
            return false;
        }

    }
    u_long on = TRUE;
    if (0 != ioctlsocket(m_SOCK, FIONBIO, &on))
    {
        OutputDebugString(L"Connection Faild ioctlsocket");
        return false;
    }



    return true;
};


BOOL NetworkProc(WPARAM wParam, LPARAM lParam)
{
    if (0 != WSAGETSELECTERROR(lParam))
    {
        return FALSE;
    }

    switch (WSAGETSELECTEVENT(lParam))
    {
    case FD_CONNECT:
    {
        return TRUE;
    }
    case FD_CLOSE:
    {
        return TRUE;
    }
    case FD_READ:
    {
        return TRUE;
    }
    case FD_WRITE:
    {
        return TRUE;
    }


    }

    return TRUE;
}

BOOL networkClean()
{
    closesocket(m_SOCK);
    WSACleanup();
    return 0;
}

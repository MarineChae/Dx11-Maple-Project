#define  _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <WinSock2.h>
#include <windows.h>
#include<vector>
#include<iostream>
#pragma comment (lib , "ws2_32.lib")

int main()
{
    int silze = 0;
    std::cin >> silze;
    for (int i = 0; i < silze; ++i)
    {


        WSADATA wsa;
        if (0 != WSAStartup(MAKEWORD(2, 2), &wsa))
        {
            return false;
        }

        SOCKET m_SOCK = socket(AF_INET, SOCK_STREAM, 0);
        if (INVALID_SOCKET == m_SOCK)
        {
            return false;
        }
        SOCKADDR_IN sa;

        sa.sin_family = AF_INET;
        sa.sin_addr.s_addr = inet_addr("14.34.20.48");
        sa.sin_port = htons(12345);

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
    }

    while (1)
    {
        int a = 0;
        std::cin >> a;
    }


	return 0;
}
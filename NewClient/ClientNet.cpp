#include "ClientNet.h"
#include"MyWindow.h"
int ClientNet::ConnentNetWork()
{
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);


   m_SOCK = socket(AF_INET, SOCK_STREAM, 0);
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
        if(err == WSAEWOULDBLOCK)
        {
            OutputDebugString(L"WSAEWOULDBLOCK");
        }
        else
        {
            OutputDebugString(L"Connection Faild");
        }
       
    }
    u_long on = TRUE;
    ioctlsocket(m_SOCK, FIONBIO, &on);





    return 0;
}

#include "Netstd.h"
#include "IOCPServer.h"



int main()
{
	
	IOCPServer::GetInstance().Init();

    

    SOCKADDR_IN ClientAddr;
    SOCKET ClientSocket;

    printf("서버 작동 시작\n");
    while (1)
    {
        int addlen = sizeof(ClientAddr);
        ClientSocket = accept(IOCPServer::GetInstance().GetSocket(), (SOCKADDR*)&ClientAddr, &addlen);
        if (ClientSocket == SOCKET_ERROR)
        {
            int iErr = WSAGetLastError();
            if (iErr != WSAEWOULDBLOCK)
            {
                break;
            }
        }
        else
        {


            printf("클라이언트 접속 IP: %s Port:%d\n", inet_ntoa(ClientAddr.sin_addr), ntohs(ClientAddr.sin_port));
            while (1)
            {
                char buf[256] = { 0 , };
                int iReciveByte = recv(ClientSocket, buf, 256, 0);
                if (iReciveByte == SOCKET_ERROR)
                {
                    int iErr = WSAGetLastError();
                    if (iErr != WSAEWOULDBLOCK)
                    {
                        break;
                    }
                }
                else
                {
                    if (iReciveByte == 0) break;
                    if (iReciveByte == SOCKET_ERROR)break;
                    printf("[받음] : %s \n", buf);

                    int iLen = strlen(buf);
                    int iSendByte = send(ClientSocket, buf, iLen, 0);
                    if (iSendByte == SOCKET_ERROR)
                    {
                        int iErr = WSAGetLastError();
                        if (iErr != WSAEWOULDBLOCK)
                        {
                            break;
                        }
                    }
                    printf("%d 바이트를 전송했습니다.", iSendByte);

                }
            }
            closesocket(ClientSocket);
            printf("클라이언트 접속해제 IP: %s Port:%d", inet_ntoa(ClientAddr.sin_addr), ntohs(ClientAddr.sin_port));

        }

    }

    closesocket(IOCPServer::GetInstance().GetSocket());
    //2)윈속 해제
    WSACleanup();








	return 0;

}
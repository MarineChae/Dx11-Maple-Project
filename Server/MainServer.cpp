#include "Netstd.h"
#include "IOCPServer.h"



int main()
{
	
	IOCPServer::GetInstance().Init();

    

    SOCKADDR_IN ClientAddr;
    SOCKET ClientSocket;

    printf("���� �۵� ����\n");
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


            printf("Ŭ���̾�Ʈ ���� IP: %s Port:%d\n", inet_ntoa(ClientAddr.sin_addr), ntohs(ClientAddr.sin_port));
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
                    printf("[����] : %s \n", buf);

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
                    printf("%d ����Ʈ�� �����߽��ϴ�.", iSendByte);

                }
            }
            closesocket(ClientSocket);
            printf("Ŭ���̾�Ʈ �������� IP: %s Port:%d", inet_ntoa(ClientAddr.sin_addr), ntohs(ClientAddr.sin_port));

        }

    }

    closesocket(IOCPServer::GetInstance().GetSocket());
    //2)���� ����
    WSACleanup();








	return 0;

}
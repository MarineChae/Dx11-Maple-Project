#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include<WinSock2.h>

const short port = 12345;


int main()
{



    //1)���� �ʱ�ȭ
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    //                      IP                   TCP
    // SOCKET SockTCP = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    // SOCKET SockUDP = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP);
    // Stream �� ������ TCP DGRAM�� UDP
    // ���� �ڿ� ���� ���� �׳� 0������ �˾Ƽ� ����
    // TCP (stream) �� �����͸� �ɰ��� �ϳ��� ���۵� Ex) 100����Ʈ ũ��� 10����Ʈ�������� ���·� ����
    // �ɰ����� �����Ͱ� ���۵Ǳ� ������ ������ ���� ���� �����͸� ���� �޾ƾ��� (������ �����)
    // �����ͺ��� �������� ũ�Ⱑ ���� ���۵� �� �����Ͱ� ���۵�
    //  
    // UDT �� �����Ͱ� �ѹ��� ���۵� ,�����͸� ���۹޴� ������ ������ ���� ����.

    SOCKET Sock = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN sa;

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr("127.0.0.1");
    sa.sin_port = htons(port);


    int ret = connect(Sock, (SOCKADDR*)&sa, sizeof(sa));
    if (ret == 0)
    {
        printf("���Ӽ��� IP: %s Port:%d", inet_ntoa(sa.sin_addr), ntohs(sa.sin_port));
    }
    else
    {
        printf("���� ����");
        return -1;
    }

    char buf[256] = { 0 , };
    int iLen = strlen(buf);
    int iSendByte = 0;
    while (1)
    {
        ZeroMemory(buf, sizeof(char) * 256);
        fgets(buf, 256, stdin);
        if (buf[0] == '\n') break;
        iLen = strlen(buf);
        iSendByte = send(Sock, buf, iLen, 0);
        printf("%d ����Ʈ�� �����߽��ϴ�.", iSendByte);

        char recvbuf[256] = { 0 , };
        int iReciveByte = recv(Sock, recvbuf, 256, 0);
        printf("[����] : %s \n", recvbuf);
    }


    closesocket(Sock);


    //2)���� ����
    WSACleanup();



    return 0;

}


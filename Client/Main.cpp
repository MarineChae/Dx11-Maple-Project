#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <iostream>
#include<WinSock2.h>

const short port = 12345;


int main()
{



    //1)윈속 초기화
    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    //                      IP                   TCP
    // SOCKET SockTCP = socket(AF_INET,SOCK_STREAM,IPPROTO_TCP);
    // SOCKET SockUDP = socket(AF_INET,SOCK_DGRAM, IPPROTO_UDP);
    // Stream 은 무조건 TCP DGRAM은 UDP
    // 굳이 뒤에 넣지 말고 그냥 0넣으면 알아서 적용
    // TCP (stream) 은 데이터를 쪼개서 하나씩 전송됨 Ex) 100바이트 크기는 10바이트씩과같은 형태로 전송
    // 쪼개져서 데이터가 전송되기 때문에 무조건 먼저 보낸 데이터를 먼저 받아야함 (순서가 보장됨)
    // 데이터보다 데이터의 크기가 먼저 전송된 후 데이터가 전송됨
    //  
    // UDT 는 데이터가 한번에 전송됨 ,데이터를 전송받는 순서가 정해저 있지 않음.

    SOCKET Sock = socket(AF_INET, SOCK_STREAM, 0);

    SOCKADDR_IN sa;

    sa.sin_family = AF_INET;
    sa.sin_addr.s_addr = inet_addr("127.0.0.1");
    sa.sin_port = htons(port);


    int ret = connect(Sock, (SOCKADDR*)&sa, sizeof(sa));
    if (ret == 0)
    {
        printf("접속성공 IP: %s Port:%d", inet_ntoa(sa.sin_addr), ntohs(sa.sin_port));
    }
    else
    {
        printf("접속 실패");
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
        printf("%d 바이트를 전송했습니다.", iSendByte);

        char recvbuf[256] = { 0 , };
        int iReciveByte = recv(Sock, recvbuf, 256, 0);
        printf("[받음] : %s \n", recvbuf);
    }


    closesocket(Sock);


    //2)윈속 해제
    WSACleanup();



    return 0;

}


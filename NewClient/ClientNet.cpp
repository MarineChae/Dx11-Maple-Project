#include"pch.h"
#include "ClientNet.h"
#include"MyWindow.h"
#include"Packet.h"
#include"StreamPacket.h"
#include"Protocol.h"
#include"PacketProc.h"



SOCKET       m_SOCK;
short        m_PortNum = 12345;
StreamPacket NetSendQ;
StreamPacket NetRecvQ;
char		 NetRecvWSABuff[NETWORK_WSABUFF_SIZE];
char		 NetSendWSABuff[NETWORK_WSABUFF_SIZE];


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
    sa.sin_addr.s_addr = inet_addr("25.31.78.91");
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
        NetRecvEvent();
        return TRUE;
    }
    case FD_WRITE:
    {


        NetSendEvent();

        return TRUE;
    }


    }
     
    return TRUE;
}

BOOL networkClean()
{
    closesocket(m_SOCK);
    WSACleanup();
    return TRUE;
}

BOOL NetSendEvent()
{
    DWORD Res;
    DWORD SendSize;
    DWORD Flag = 0;

    WSABUF WsaBuff;

    if (0 >= NetSendQ.GetUseSize())
        return TRUE;


    SendSize = NetSendQ.GetUseSize();
    //최대 NETWORK_WSABUFF_SIZE만큼만 전송가능
    SendSize = min(NETWORK_WSABUFF_SIZE, SendSize);

    NetSendQ.Peek(NetSendWSABuff, SendSize);

    WsaBuff.buf = NetSendWSABuff;
    WsaBuff.len = SendSize;

    MyOV* ov = new MyOV(MyOV::MODE_SEND);


    Res = WSASend(m_SOCK, &WsaBuff, 1, &SendSize, Flag, NULL, NULL);
    

    if (SOCKET_ERROR == Res)
    {

        if (WSAEWOULDBLOCK == GetLastError())
        {
            delete ov;
            return TRUE;
        }
        return FALSE;

    }
    else
    {
        if (WsaBuff.len < SendSize)
        {
            delete ov;
            return FALSE;
        }
        else
        {
            NetSendQ.RemoveData(SendSize);
        }
    }
    
    return TRUE;
}

DWORD NetCompleteRecvPacket()
{
    PACKET_HEADER ph;

    int iRecvsize;
    BYTE byEndCode;

    iRecvsize = NetRecvQ.GetUseSize();


    if (PACKET_HEADER_SIZE > iRecvsize)
        return 1;


    NetRecvQ.Peek((char*)&ph, PACKET_HEADER_SIZE);

    if (NETWORK_PACKET_CODE != ph.PacketCode)
        return 0xff;

    //큐에 저장된 데이터가 패킷의 크기만큼 들어있는지 확인
    //end코드까지 포함한 사이즈  
    //사이즈가 작다면 패킷전송이 완료되지 않음 다시처리
    if (ph.PacketSize + PACKET_HEADER_SIZE + 1 > iRecvsize)
        return 1;

    //헤더 정보를 빼왔으므로 헤더크기만큼 데이터 지워줌
    NetRecvQ.RemoveData(PACKET_HEADER_SIZE);

    std::shared_ptr<Packet> pack = std::make_shared<Packet>();
    //큐에서 패킷 크기만큼 데이터를 빼온다
    if(!NetRecvQ.Get(pack->GetBufferPointer(),ph.PacketSize))
        return 0xff;


    if(!NetRecvQ.Get((char*)&byEndCode,1))
        return 0xff;
    if (NETWORK_PACKET_END != byEndCode)
        return 0xff;

    //패킷에 데이터를 넣어주었으므로 읽기 위치를 이동
    pack->MoveWritePos(ph.PacketSize);

    if (!PacketProc(ph.PacketType, pack))
        return 0xff;


    return 0;
}

BOOL NetRecvEvent()
{
    DWORD dwResult;
    DWORD dwRecvSize;
    DWORD dwFlag = 0;

    WSABUF WsaBuff;
    
    WsaBuff.buf = NetRecvWSABuff;
    WsaBuff.len = NETWORK_WSABUFF_SIZE;

    dwResult = WSARecv(m_SOCK, &WsaBuff, 1, &dwRecvSize, &dwFlag, NULL, NULL);

    if (SOCKET_ERROR == dwResult)
    {
        int err = WSAGetLastError();

        if (err != WSAEWOULDBLOCK)
            return FALSE;
        else
            return TRUE;
    }

    if (0 < dwRecvSize)
    {
        NetRecvQ.Put(NetRecvWSABuff, dwRecvSize);

        while (1)
        {
            dwResult = NetCompleteRecvPacket();

            if (1 == dwResult)
                break;

            if (0xff == dwResult)
                return FALSE;

        }



    }


    return 0;
}

BOOL PacketProc(BYTE byPacketType, std::shared_ptr<Packet> pack)
{
    switch (byPacketType)
    {
    case PACKET_CS_MOVE_START :
        PacketProc_MoveStart(pack);
        break;

    case PACKET_CS_MOVE_END :
        PacketProc_MoveEnd(pack);
        break;
    case PACKET_CS_CREATE_MY_CHARACTER:
        PacketProc_CreateMyCharacter(pack);
        break;
    case PACKET_CS_CREATE_OTHER_CHARACTER:
        PacketProc_CreateOtherCharacter(pack);
        break;
    case PACKET_CS_DISCONNECT_CHARACTER:
        PacketProc_DisconnectOtherCharacter(pack);
        break;
    case PACKET_CS_SCENE_CHANGE:
        PacketProc_SceneChange(pack);
        break;
    case PACKET_CS_CREATE_MONSTER:
        PacketProc_CreateMonster(pack);
        break;
    case PACKET_CS_UPDATE_MONSTER_STATE:
         PacketProc_UpdateMonster(pack);
        break;
    case PACKET_CS_CHRACTER_ATTACK:
        PacketProc_Attack(pack);
        break;
    case PACKET_CS_SPAWN_OBJECT:
        PacketProc_CreateObject(pack);
        break;
        

    }




    return TRUE;
}

BOOL NetSendPacket(std::shared_ptr<Packet> packet)
{

    NetSendQ.Put(packet->GetBufferPointer(), packet->GetDataSize());

    if (!NetSendEvent())
    {
        closesocket(m_SOCK);
    }

    return TRUE;
}




static int RecvWork()
{
    


    return -1;
}